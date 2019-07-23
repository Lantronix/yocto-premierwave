/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2012  Intel Corporation. All rights reserved.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <wordexp.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <glib.h>

#include "util.h"
#include "gdbus.h"
#include "uuid.h"
#include "agent.h"
#include "display.h"
#include "gatt.h"
#include "advertising.h"

/* String display constants */
#define COLORED_NEW	COLOR_GREEN "NEW" COLOR_OFF
#define COLORED_CHG	COLOR_YELLOW "CHG" COLOR_OFF
#define COLORED_DEL	COLOR_RED "DEL" COLOR_OFF

#define PROMPT_ON	COLOR_BLUE "[bluetooth]" COLOR_OFF "# "
#define PROMPT_OFF	"Waiting to connect to bluetoothd..."

#define VERSION "5.43"

static GMainLoop *main_loop;
static DBusConnection *dbus_conn;

static GDBusProxy *agent_manager;
static char *auto_register_agent = NULL;

struct adapter {
	GDBusProxy *proxy;
	GList *devices;
};

static struct adapter *default_ctrl;
static GDBusProxy *default_dev;
static GDBusProxy *default_attr;
static GDBusProxy *ad_manager;
static GList *ctrl_list;

static guint input = 0;
static void cmd_info(const char *arg);
uint16_t manufacture_key = 0;
uint8_t manu_or_service_data[30] = {0};
uint8_t manu_or_service_data_index;
static uint8_t multi_byte_value_flag = 0;
static uint8_t manufacture_first_value_flag = 0;
uint8_t manufature_first_value_byte = 0;
bool is_EddystoneBeacon = false;
static const char * const agent_arguments[] = {
	"on",
	"off",
	"DisplayOnly",
	"DisplayYesNo",
	"KeyboardDisplay",
	"KeyboardOnly",
	"NoInputNoOutput",
	NULL
};

static const char * const ad_arguments[] = {
	"on",
	"off",
	"peripheral",
	"broadcast",
	NULL
};
static void proxy_leak(gpointer data)
{
	printf("Leaking proxy %p\n", data);
}

static gboolean input_handler(GIOChannel *channel, GIOCondition condition,
							gpointer user_data)
{
	if (condition & G_IO_IN) {
		rl_callback_read_char();
		return TRUE;
	}

	if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL)) {
		g_main_loop_quit(main_loop);
		return FALSE;
	}

	return TRUE;
}

static guint setup_standard_input(void)
{
	GIOChannel *channel;
	guint source;

	channel = g_io_channel_unix_new(fileno(stdin));

	source = g_io_add_watch(channel,
				G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
				input_handler, NULL);

	g_io_channel_unref(channel);

	return source;
}
static void connect_handler(DBusConnection *connection, void *user_data)
{
	rl_set_prompt(PROMPT_ON);
	printf("\r");
	rl_on_new_line();
	rl_redisplay();
}
static void disconnect_handler(DBusConnection *connection, void *user_data)
{
	if (input > 0) {
		g_source_remove(input);
		input = 0;
	}

	rl_set_prompt(PROMPT_OFF);
	printf("\r");
	rl_on_new_line();
	rl_redisplay();

	g_list_free_full(ctrl_list, proxy_leak);
	ctrl_list = NULL;

	default_ctrl = NULL;
}
static void print_adapter(GDBusProxy *proxy, const char *description)
{
	DBusMessageIter iter;
	const char *address, *name;

	if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE)
		return;

	dbus_message_iter_get_basic(&iter, &address);

	if (g_dbus_proxy_get_property(proxy, "Alias", &iter) == TRUE)
		dbus_message_iter_get_basic(&iter, &name);
	else
		name = "<unknown>";

	rl_printf("%s%s%sController %s %s %s\n",
				description ? "[" : "",
				description ? : "",
				description ? "] " : "",
				address, name,
				default_ctrl &&
				default_ctrl->proxy == proxy ?
				"[default]" : "");

}

static void print_device(GDBusProxy *proxy, const char *description)
{
	DBusMessageIter iter;
	const char *address, *name;

	if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE)
		return;

	dbus_message_iter_get_basic(&iter, &address);

	if (g_dbus_proxy_get_property(proxy, "Alias", &iter) == TRUE)
		dbus_message_iter_get_basic(&iter, &name);
	else
		name = "<unknown>";

	rl_printf("%s%s%sDevice %s %s\n",
				description ? "[" : "",
				description ? : "",
				description ? "] " : "",
				address, name);
}
static void get_key_iter(const char *label, const char *name,
                                                DBusMessageIter *iter)
{
        dbus_bool_t valbool;
        dbus_uint32_t valu32;
        dbus_uint16_t valu16;
        dbus_int16_t vals16;
        unsigned char byte;
        const char *valstr;
        DBusMessageIter subiter;
        char *entry;

        if (iter == NULL) {
                rl_printf("%s%s is nil\n", label, name);
                return;
        }

        switch (dbus_message_iter_get_arg_type(iter)) {
        case DBUS_TYPE_INVALID:
                rl_printf("%s%s is invalid\n", label, name);
                break;
        case DBUS_TYPE_STRING:
        case DBUS_TYPE_OBJECT_PATH:
                dbus_message_iter_get_basic(iter, &valstr);
                //rl_printf("%s%s: %s\n", label, name, valstr);
                break;
        case DBUS_TYPE_BOOLEAN:
                dbus_message_iter_get_basic(iter, &valbool);
                /*rl_printf("%s%s: %s\n", label, name,
                                        valbool == TRUE ? "yes" : "no");*/
                break;
        case DBUS_TYPE_UINT32:
                dbus_message_iter_get_basic(iter, &valu32);
                //rl_printf("%s%s: 0x%06x\n", label, name, valu32);
                break;
        case DBUS_TYPE_UINT16:
                dbus_message_iter_get_basic(iter, &valu16);
                manufacture_key = (uint16_t)valu16; 
                //rl_printf("%s%s: 0x%04x\n", label, name, valu16);
                break;
        case DBUS_TYPE_INT16:
                dbus_message_iter_get_basic(iter, &vals16);
                //rl_printf("%s%s: %d\n", label, name, vals16);
                break;
        case DBUS_TYPE_BYTE:
                dbus_message_iter_get_basic(iter, &byte);
                if(manufacture_first_value_flag)
                {
                    manufature_first_value_byte = byte;
                    //rl_printf("manufature_first_value_byte : 0x%02x\n", manufature_first_value_byte ); 
                    manufacture_first_value_flag = 0;
                }
                //rl_printf("%s%s: 0x%02x\n", label, name, byte);
                break;
        case DBUS_TYPE_VARIANT:
                dbus_message_iter_recurse(iter, &subiter);
                get_key_iter(label, name, &subiter);
                break;
        case DBUS_TYPE_ARRAY:
                dbus_message_iter_recurse(iter, &subiter);
                while (dbus_message_iter_get_arg_type(&subiter) !=
                                                        DBUS_TYPE_INVALID) {
                        get_key_iter(label, name, &subiter);
                        dbus_message_iter_next(&subiter);
                }
                break;
        case DBUS_TYPE_DICT_ENTRY:
                dbus_message_iter_recurse(iter, &subiter);
                entry = g_strconcat(name, " Key", NULL);
                get_key_iter(label, entry, &subiter);
                g_free(entry);
                
                entry = g_strconcat(name, " Value", NULL);
                dbus_message_iter_next(&subiter);
                manufacture_first_value_flag = 1;
                get_key_iter(label, entry, &subiter);
                g_free(entry);
                break;
        default:
                rl_printf("%s%s has unsupported type\n", label, name);
                break;
        }
}
static void print_iter(const char *label, const char *name,
						DBusMessageIter *iter)
{
	dbus_bool_t valbool;
	dbus_uint32_t valu32;
	dbus_uint16_t valu16;
	dbus_int16_t vals16;
	unsigned char byte;
	const char *valstr;
	DBusMessageIter subiter;
	char *entry;

	if (iter == NULL) {
		rl_printf("%s%s is nil\n", label, name);
		return;
	}

	switch (dbus_message_iter_get_arg_type(iter)) {
	case DBUS_TYPE_INVALID:
		rl_printf("%s%s is invalid\n", label, name);
		break;
	case DBUS_TYPE_STRING:
	case DBUS_TYPE_OBJECT_PATH:
		dbus_message_iter_get_basic(iter, &valstr);
		rl_printf("%s%s: %s\n", label, name, valstr);
		break;
	case DBUS_TYPE_BOOLEAN:
		dbus_message_iter_get_basic(iter, &valbool);
		rl_printf("%s%s: %s\n", label, name,
					valbool == TRUE ? "yes" : "no");
		break;
	case DBUS_TYPE_UINT32:
		dbus_message_iter_get_basic(iter, &valu32);
		rl_printf("%s%s: 0x%06x\n", label, name, valu32);
		break;
	case DBUS_TYPE_UINT16:
		dbus_message_iter_get_basic(iter, &valu16);
		rl_printf("%s%s: 0x%04x\n", label, name, valu16);
		break;
	case DBUS_TYPE_INT16:
		dbus_message_iter_get_basic(iter, &vals16);
		rl_printf("%s%s: %d\n", label, name, vals16);
		break;
	case DBUS_TYPE_BYTE:
		dbus_message_iter_get_basic(iter, &byte);
		rl_printf("%s%s: 0x%02x\n", label, name, byte);
                if(multi_byte_value_flag)
                {
                     manu_or_service_data[manu_or_service_data_index++] = byte;
                }
		break;
	case DBUS_TYPE_VARIANT:
		dbus_message_iter_recurse(iter, &subiter);
		print_iter(label, name, &subiter);
		break;
	case DBUS_TYPE_ARRAY:
		dbus_message_iter_recurse(iter, &subiter);
		while (dbus_message_iter_get_arg_type(&subiter) !=
							DBUS_TYPE_INVALID) {
			print_iter(label, name, &subiter);
			dbus_message_iter_next(&subiter);
		}
		break;
	case DBUS_TYPE_DICT_ENTRY:
		dbus_message_iter_recurse(iter, &subiter);
		entry = g_strconcat(name, " Key", NULL);
		print_iter(label, entry, &subiter);
		g_free(entry);

		entry = g_strconcat(name, " Value", NULL);
		dbus_message_iter_next(&subiter);
		print_iter(label, entry, &subiter);
		g_free(entry);
		break;
	default:
		rl_printf("%s%s has unsupported type\n", label, name);
		break;
	}
}
static void get_key_property(GDBusProxy *proxy)
{
        DBusMessageIter iter;

        if (g_dbus_proxy_get_property(proxy, "ManufacturerData", &iter) == FALSE)
                return;

        get_key_iter("\t", "ManufacturerData", &iter);
}
static void print_property(GDBusProxy *proxy, const char *name)
{
	DBusMessageIter iter;

	if (g_dbus_proxy_get_property(proxy, name, &iter) == FALSE)
		return;

	print_iter("\t", name, &iter);
}
static char *get_uuids(GDBusProxy *proxy)
{
        DBusMessageIter iter, value;

        if (g_dbus_proxy_get_property(proxy, "UUIDs", &iter) == FALSE)
                return NULL;

        dbus_message_iter_recurse(&iter, &value);
        char match_uuid [40];
        match_uuid[0] = '\0';
        while (dbus_message_iter_get_arg_type(&value) == DBUS_TYPE_STRING) {
                const char *uuid, *text;

                dbus_message_iter_get_basic(&value, &uuid);

                text = uuidstr_to_str(uuid);
                if (text) {
                        char str[26];
                        unsigned int n;

                        str[sizeof(str) - 1] = '\0';

                        n = snprintf(str, sizeof(str), "%s", text);
                        if (n > sizeof(str) - 1) {
                                str[sizeof(str) - 2] = '.';
                                str[sizeof(str) - 3] = '.';
                                if (str[sizeof(str) - 4] == ' ')
                                        str[sizeof(str) - 4] = '.';

                                n = sizeof(str) - 1;
                        }

                        /*rl_printf("\tUUID: %s%*c(%s)\n",
                                                str, 26 - n, ' ', uuid);*/
                } /*else
                        rl_printf("\tUUID: %*c(%s)\n", 26, ' ', uuid);*/
                if(!memcmp(uuid, "0000feaa-0000-1000-8000-00805f9b34fb", 36))
                {     
                     memcpy(match_uuid, uuid, 36);
                     match_uuid[36] = '\0';
                }
                dbus_message_iter_next(&value); 
        }
        return match_uuid;
}
static void print_uuids(GDBusProxy *proxy)
{
	DBusMessageIter iter, value;

	if (g_dbus_proxy_get_property(proxy, "UUIDs", &iter) == FALSE)
		return;

	dbus_message_iter_recurse(&iter, &value);

	while (dbus_message_iter_get_arg_type(&value) == DBUS_TYPE_STRING) {
		const char *uuid, *text;

		dbus_message_iter_get_basic(&value, &uuid);

		text = uuidstr_to_str(uuid);
		if (text) {
			char str[26];
			unsigned int n;

			str[sizeof(str) - 1] = '\0';

			n = snprintf(str, sizeof(str), "%s", text);
			if (n > sizeof(str) - 1) {
				str[sizeof(str) - 2] = '.';
				str[sizeof(str) - 3] = '.';
				if (str[sizeof(str) - 4] == ' ')
					str[sizeof(str) - 4] = '.';

				n = sizeof(str) - 1;
			}

			rl_printf("\tUUID: %s%*c(%s)\n",
						str, 26 - n, ' ', uuid);
		} else
			rl_printf("\tUUID: %*c(%s)\n", 26, ' ', uuid);
                if(!memcmp(uuid, "0000feaa-0000-1000-8000-00805f9b34fb", 36))
                    is_EddystoneBeacon = true; 

		dbus_message_iter_next(&value);
	}
}

static gboolean device_is_child(GDBusProxy *device, GDBusProxy *master)
{
	DBusMessageIter iter;
	const char *adapter, *path;

	if (!master)
		return FALSE;

	if (g_dbus_proxy_get_property(device, "Adapter", &iter) == FALSE)
		return FALSE;

	dbus_message_iter_get_basic(&iter, &adapter);
	path = g_dbus_proxy_get_path(master);

	if (!strcmp(path, adapter))
		return TRUE;

	return FALSE;
}

static gboolean service_is_child(GDBusProxy *service)
{
	GList *l;
	DBusMessageIter iter;
	const char *device, *path;

	if (g_dbus_proxy_get_property(service, "Device", &iter) == FALSE)
		return FALSE;

	dbus_message_iter_get_basic(&iter, &device);

	if (!default_ctrl)
		return FALSE;

	for (l = default_ctrl->devices; l; l = g_list_next(l)) {
		struct GDBusProxy *proxy = l->data;

		path = g_dbus_proxy_get_path(proxy);

		if (!strcmp(path, device))
			return TRUE;
	}

	return FALSE;
}
static struct adapter *find_parent(GDBusProxy *device)
{
	GList *list;

	for (list = g_list_first(ctrl_list); list; list = g_list_next(list)) {
		struct adapter *adapter = list->data;

		if (device_is_child(device, adapter->proxy) == TRUE)
			return adapter;
	}
	return NULL;
}

static void set_default_device(GDBusProxy *proxy, const char *attribute)
{
	char *desc = NULL;
	DBusMessageIter iter;
	const char *path;

	default_dev = proxy;

	if (proxy == NULL) {
		default_attr = NULL;
		goto done;
	}

	if (!g_dbus_proxy_get_property(proxy, "Alias", &iter)) {
		if (!g_dbus_proxy_get_property(proxy, "Address", &iter))
			goto done;
	}

	path = g_dbus_proxy_get_path(proxy);

	dbus_message_iter_get_basic(&iter, &desc);
	desc = g_strdup_printf(COLOR_BLUE "[%s%s%s]" COLOR_OFF "# ", desc,
				attribute ? ":" : "",
				attribute ? attribute + strlen(path) : "");

done:
	rl_set_prompt(desc ? desc : PROMPT_ON);
	printf("\r");
	rl_on_new_line();
	g_free(desc);
}
static void device_added(GDBusProxy *proxy)
{
	DBusMessageIter iter;
	struct adapter *adapter = find_parent(proxy);

	if (!adapter) {
		/* TODO: Error */
		return;
	}

        get_key_property(proxy);
        if(!memcmp(get_uuids(proxy), "0000feaa-0000-1000-8000-00805f9b34fb", 36) || (manufacture_key == 0x4c && manufature_first_value_byte == 0x02))
	{
          
            adapter->devices = g_list_append(adapter->devices, proxy);
            const char *address;
            DBusMessageIter addr_iter;
            if (g_dbus_proxy_get_property(proxy, "Address",
                 &addr_iter) == TRUE) {

                                dbus_message_iter_get_basic(&addr_iter,
                                                                &address);
            }
               
            print_device(proxy, COLORED_NEW);
            cmd_info(address);
            manufacture_key = 0;
            manufature_first_value_byte = 0x00;
        }
        else
        {
            manufacture_key = 0;
            manufature_first_value_byte = 0x00;
        }
	if (default_dev)
		return;

	if (g_dbus_proxy_get_property(proxy, "Connected", &iter)) {
		dbus_bool_t connected;

		dbus_message_iter_get_basic(&iter, &connected);

		if (connected)
			set_default_device(proxy, NULL);
	}
}

static void adapter_added(GDBusProxy *proxy)
{
	struct adapter *adapter = g_malloc0(sizeof(struct adapter));

	adapter->proxy = proxy;
	ctrl_list = g_list_append(ctrl_list, adapter);

	if (!default_ctrl)
		default_ctrl = adapter;

	print_adapter(proxy, COLORED_NEW);
}

static void proxy_added(GDBusProxy *proxy, void *user_data)
{
	const char *interface;

	interface = g_dbus_proxy_get_interface(proxy);

	if (!strcmp(interface, "org.bluez.Device1")) {
		device_added(proxy);
	} else if (!strcmp(interface, "org.bluez.Adapter1")) {
		adapter_added(proxy);
	} else if (!strcmp(interface, "org.bluez.AgentManager1")) {
		if (!agent_manager) {
			agent_manager = proxy;

			if (auto_register_agent)
				agent_register(dbus_conn, agent_manager,
							auto_register_agent);
		}
	} else if (!strcmp(interface, "org.bluez.GattService1")) {
		if (service_is_child(proxy))
			gatt_add_service(proxy);
	} else if (!strcmp(interface, "org.bluez.GattCharacteristic1")) {
		gatt_add_characteristic(proxy);
	} else if (!strcmp(interface, "org.bluez.GattDescriptor1")) {
		gatt_add_descriptor(proxy);
	} else if (!strcmp(interface, "org.bluez.GattManager1")) {
		gatt_add_manager(proxy);
	} else if (!strcmp(interface, "org.bluez.LEAdvertisingManager1")) {
		ad_manager = proxy;
	}
}
static void set_default_attribute(GDBusProxy *proxy)
{
	const char *path;

	default_attr = proxy;

	path = g_dbus_proxy_get_path(proxy);

	set_default_device(default_dev, path);
}

static void device_removed(GDBusProxy *proxy)
{
	struct adapter *adapter = find_parent(proxy);
	if (!adapter) {
		/* TODO: Error */
		return;
	}

	adapter->devices = g_list_remove(adapter->devices, proxy);

	print_device(proxy, COLORED_DEL);

	if (default_dev == proxy)
		set_default_device(NULL, NULL);
}

static void adapter_removed(GDBusProxy *proxy)
{
	GList *ll;

	for (ll = g_list_first(ctrl_list); ll; ll = g_list_next(ll)) {
		struct adapter *adapter = ll->data;

		if (adapter->proxy == proxy) {
			print_adapter(proxy, COLORED_DEL);

			if (default_ctrl && default_ctrl->proxy == proxy) {
				default_ctrl = NULL;
				set_default_device(NULL, NULL);
			}

			ctrl_list = g_list_remove_link(ctrl_list, ll);
			g_list_free(adapter->devices);
			g_free(adapter);
			g_list_free(ll);
			return;
		}
	}
}
static void proxy_removed(GDBusProxy *proxy, void *user_data)
{
	const char *interface;

	interface = g_dbus_proxy_get_interface(proxy);

	if (!strcmp(interface, "org.bluez.Device1")) {
		device_removed(proxy);
	} else if (!strcmp(interface, "org.bluez.Adapter1")) {
		adapter_removed(proxy);
	} else if (!strcmp(interface, "org.bluez.AgentManager1")) {
		if (agent_manager == proxy) {
			agent_manager = NULL;
			if (auto_register_agent)
				agent_unregister(dbus_conn, NULL);
		}
	} else if (!strcmp(interface, "org.bluez.GattService1")) {
		gatt_remove_service(proxy);

		if (default_attr == proxy)
			set_default_attribute(NULL);
	} else if (!strcmp(interface, "org.bluez.GattCharacteristic1")) {
		gatt_remove_characteristic(proxy);

		if (default_attr == proxy)
			set_default_attribute(NULL);
	} else if (!strcmp(interface, "org.bluez.GattDescriptor1")) {
		gatt_remove_descriptor(proxy);

		if (default_attr == proxy)
			set_default_attribute(NULL);
	} else if (!strcmp(interface, "org.bluez.GattManager1")) {
		gatt_remove_manager(proxy);
	} else if (!strcmp(interface, "org.bluez.LEAdvertisingManager1")) {
		if (ad_manager == proxy) {
			agent_manager = NULL;
			ad_unregister(dbus_conn, NULL);
		}
	}
}

static void property_changed(GDBusProxy *proxy, const char *name,
					DBusMessageIter *iter, void *user_data)
{
	const char *interface;

	interface = g_dbus_proxy_get_interface(proxy);

	if (!strcmp(interface, "org.bluez.Device1")) {
		if (default_ctrl && device_is_child(proxy,
					default_ctrl->proxy) == TRUE) {
			DBusMessageIter addr_iter;
			char *str;

			if (g_dbus_proxy_get_property(proxy, "Address",
							&addr_iter) == TRUE) {
				const char *address;

				dbus_message_iter_get_basic(&addr_iter,
								&address);
             
                                get_key_property(proxy);
                                if(!memcmp(get_uuids(proxy), "0000feaa-0000-1000-8000-00805f9b34fb", 36) || 
                                    (manufacture_key == 0x4c && manufature_first_value_byte == 0x02))
                                {
				     str = g_strdup_printf("[" COLORED_CHG
						"] Device %s ", address);
                                     manufacture_key = 0;
                                     manufature_first_value_byte = 0x00;
                                }
                                else 
                                {
                                     manufature_first_value_byte = 0x00;
                                     manufacture_key = 0;
                                     return;
                                }
			} else
				str = g_strdup("");

			if (strcmp(name, "Connected") == 0) {
				dbus_bool_t connected;

				dbus_message_iter_get_basic(iter, &connected);

				if (connected && default_dev == NULL)
					set_default_device(proxy, NULL);
				else if (!connected && default_dev == proxy)
					set_default_device(NULL, NULL);
			}

			print_iter(str, name, iter);
			g_free(str);
		}
	} else if (!strcmp(interface, "org.bluez.Adapter1")) {
		DBusMessageIter addr_iter;
		char *str;

		if (g_dbus_proxy_get_property(proxy, "Address",
						&addr_iter) == TRUE) {
			const char *address;

			dbus_message_iter_get_basic(&addr_iter, &address);
			str = g_strdup_printf("[" COLORED_CHG
						"] Controller %s ", address);
		} else
			str = g_strdup("");

		print_iter(str, name, iter);
		g_free(str);
	} else if (proxy == default_attr) {
		char *str;

		str = g_strdup_printf("[" COLORED_CHG "] Attribute %s ",
						g_dbus_proxy_get_path(proxy));

		print_iter(str, name, iter);
		g_free(str);
	}
}
static void message_handler(DBusConnection *connection,
					DBusMessage *message, void *user_data)
{
	rl_printf("[SIGNAL] %s.%s\n", dbus_message_get_interface(message),
					dbus_message_get_member(message));
}
static struct adapter *find_ctrl_by_address(GList *source, const char *address)
{
	GList *list;

	for (list = g_list_first(source); list; list = g_list_next(list)) {
		struct adapter *adapter = list->data;
		DBusMessageIter iter;
		const char *str;

		if (g_dbus_proxy_get_property(adapter->proxy,
					"Address", &iter) == FALSE)
			continue;

		dbus_message_iter_get_basic(&iter, &str);

		if (!strcmp(str, address))
			return adapter;
	}

	return NULL;
}

static GDBusProxy *find_proxy_by_address(GList *source, const char *address)
{
	GList *list;

	for (list = g_list_first(source); list; list = g_list_next(list)) {
		GDBusProxy *proxy = list->data;
		DBusMessageIter iter;
		const char *str;

		if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE)
			continue;

		dbus_message_iter_get_basic(&iter, &str);

		if (!strcmp(str, address))
			return proxy;
	}

	return NULL;
}

static gboolean check_default_ctrl(void)
{
	if (!default_ctrl) {
		rl_printf("No default controller available\n");
		return FALSE;
	}

	return TRUE;
}
static gboolean parse_argument_on_off(const char *arg, dbus_bool_t *value)
{
	if (!arg || !strlen(arg)) {
		rl_printf("Missing on/off argument\n");
		return FALSE;
	}

	if (!strcmp(arg, "on") || !strcmp(arg, "yes")) {
		*value = TRUE;
		return TRUE;
	}

	if (!strcmp(arg, "off") || !strcmp(arg, "no")) {
		*value = FALSE;
		return TRUE;
	}

	rl_printf("Invalid argument %s\n", arg);
	return FALSE;
}
static void cmd_list(const char *arg)
{
	GList *list;

	for (list = g_list_first(ctrl_list); list; list = g_list_next(list)) {
		struct adapter *adapter = list->data;
		print_adapter(adapter->proxy, NULL);
	}
}

static void cmd_show(const char *arg)
{
	struct adapter *adapter;
	GDBusProxy *proxy;
	DBusMessageIter iter;
	const char *address;

	if (!arg || !strlen(arg)) {
		if (check_default_ctrl() == FALSE)
			return;

		proxy = default_ctrl->proxy;
	} else {
		adapter = find_ctrl_by_address(ctrl_list, arg);
		if (!adapter) {
			rl_printf("Controller %s not available\n", arg);
			return;
		}
		proxy = adapter->proxy;
	}

	if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE)
		return;

	dbus_message_iter_get_basic(&iter, &address);
	rl_printf("Controller %s\n", address);

	print_property(proxy, "Name");
	print_property(proxy, "Alias");
	print_property(proxy, "Class");
	print_property(proxy, "Powered");
	print_property(proxy, "Discoverable");
	print_property(proxy, "Pairable");
	print_uuids(proxy);
	print_property(proxy, "Modalias");
	print_property(proxy, "Discovering");
}

static void cmd_select(const char *arg)
{
	struct adapter *adapter;

	if (!arg || !strlen(arg)) {
		rl_printf("Missing controller address argument\n");
		return;
	}

	adapter = find_ctrl_by_address(ctrl_list, arg);
	if (!adapter) {
		rl_printf("Controller %s not available\n", arg);
		return;
	}

	if (default_ctrl && default_ctrl->proxy == adapter->proxy)
		return;

	default_ctrl = adapter;
	print_adapter(adapter->proxy, NULL);
}

static void cmd_devices(const char *arg)
{
	GList *ll;

	if (check_default_ctrl() == FALSE)
		return;

	for (ll = g_list_first(default_ctrl->devices);
			ll; ll = g_list_next(ll)) {
		GDBusProxy *proxy = ll->data;
		print_device(proxy, NULL);
	}
}
static void start_discovery_reply(DBusMessage *message, void *user_data)
{
	dbus_bool_t enable = GPOINTER_TO_UINT(user_data);
	DBusError error;

	dbus_error_init(&error);

	if (dbus_set_error_from_message(&error, message) == TRUE) {
		rl_printf("Failed to %s discovery: %s\n",
				enable == TRUE ? "start" : "stop", error.name);
		dbus_error_free(&error);
		return;
	}

	rl_printf("Discovery %s\n", enable == TRUE ? "started" : "stopped");
}
static void cmd_scan(const char *arg)
{
	dbus_bool_t enable;
	const char *method;

	if (parse_argument_on_off(arg, &enable) == FALSE)
		return;

	if (check_default_ctrl() == FALSE)
		return;

	if (enable == TRUE)
		method = "StartDiscovery";
	else
		method = "StopDiscovery";

	if (g_dbus_proxy_method_call(default_ctrl->proxy, method,
				NULL, start_discovery_reply,
				GUINT_TO_POINTER(enable), NULL) == FALSE) {
		rl_printf("Failed to %s discovery\n",
					enable == TRUE ? "start" : "stop");
		return;
	}
}
static struct GDBusProxy *find_device(const char *arg)
{
	GDBusProxy *proxy;

	if (!arg || !strlen(arg)) {
		if (default_dev)
			return default_dev;
		rl_printf("Missing device address argument\n");
		return NULL;
	}

	if (check_default_ctrl() == FALSE)
		return NULL;

	proxy = find_proxy_by_address(default_ctrl->devices, arg);
	if (!proxy) {
		rl_printf("Device %s not available\n", arg);
		return NULL;
	}

	return proxy;
}

static void cmd_info(const char *arg)
{
	GDBusProxy *proxy;
	DBusMessageIter iter;
	const char *address;

	proxy = find_device(arg);
	if (!proxy)
		return;

	if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE)
		return;

	dbus_message_iter_get_basic(&iter, &address);
	rl_printf("Device %s\n", address);

	print_property(proxy, "Name");
	print_property(proxy, "Alias");
	print_property(proxy, "Class");
	print_property(proxy, "Appearance");
	print_property(proxy, "Icon");
	print_property(proxy, "Paired");
	print_property(proxy, "Trusted");
	print_property(proxy, "Blocked");
	print_property(proxy, "Connected");
	print_property(proxy, "LegacyPairing");
        is_EddystoneBeacon = false;
	print_uuids(proxy);
	print_property(proxy, "Modalias");
        manu_or_service_data_index = 0;
        multi_byte_value_flag = 1;        
	print_property(proxy, "ManufacturerData");
        multi_byte_value_flag = 1;
	print_property(proxy, "ServiceData");
        multi_byte_value_flag = 0;
        uint8_t index;
        rl_printf("\tManufacture/Service Data: ");
        for(index = 0; index < manu_or_service_data_index; index++)
            rl_printf("%02x", manu_or_service_data[index]);
        rl_printf("\n");
        if(is_EddystoneBeacon == true && manu_or_service_data[0] == 0x10)
        { 
            uint8_t prefix[20], site_name[50], domain[10];
            if(manu_or_service_data[2] == 0x00)
                 sprintf(prefix, "http://www.");
            else if(manu_or_service_data[2] == 0x01)
                 sprintf(prefix, "https://www.");
            else if(manu_or_service_data[2] == 0x02)
                 sprintf(prefix, "http://");
            else if(manu_or_service_data[2] == 0x03)
                 sprintf(prefix, "https://");
            else 
                 sprintf(prefix, "invalid prefix");
            if(manu_or_service_data[manu_or_service_data_index - 1] == 0x00)
                 sprintf(domain, ".com/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x00)
                 sprintf(domain, ".com/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x01)
                 sprintf(domain, ".org/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x02)
                 sprintf(domain, ".edu/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x03)
                 sprintf(domain, ".net/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x04)
                 sprintf(domain, ".info/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x05)
                 sprintf(domain, ".biz/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x06)
                 sprintf(domain, ".gov/");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x07)
                 sprintf(domain, ".com");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x08)
                 sprintf(domain, ".org");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x09)
                 sprintf(domain, ".edu");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x0a)
                 sprintf(domain, ".net");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x0b)
                 sprintf(domain, ".info");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x0c)
                 sprintf(domain, ".biz");
            else if(manu_or_service_data[manu_or_service_data_index - 1] == 0x0d)
                 sprintf(domain, ".gov");
            else
                 sprintf(domain, "");
            for(index = 0; index < (manu_or_service_data_index - 4); index++)
                 site_name[index] = manu_or_service_data[index + 3];
            if(!strcmp(domain, ""))
            {
                site_name[index] = manu_or_service_data[index + 3];
                site_name[index + 1] = '\0';
            }
            else
                 site_name[index] = '\0';
            rl_printf("\tURL : %s%s%s\n", prefix, site_name, domain);
        }
        manu_or_service_data_index = 0;
	print_property(proxy, "RSSI");
	print_property(proxy, "TxPower");
        is_EddystoneBeacon = false;
}
static void remove_device_reply(DBusMessage *message, void *user_data)
{
	DBusError error;

	dbus_error_init(&error);

	if (dbus_set_error_from_message(&error, message) == TRUE) {
		rl_printf("Failed to remove device: %s\n", error.name);
		dbus_error_free(&error);
		return;
	}

	rl_printf("Device has been removed\n");
}

static void remove_device_setup(DBusMessageIter *iter, void *user_data)
{
	const char *path = user_data;

	dbus_message_iter_append_basic(iter, DBUS_TYPE_OBJECT_PATH, &path);
}

static void remove_device(GDBusProxy *proxy)
{
	char *path;

	path = g_strdup(g_dbus_proxy_get_path(proxy));

	if (!default_ctrl)
		return;

	if (g_dbus_proxy_method_call(default_ctrl->proxy, "RemoveDevice",
						remove_device_setup,
						remove_device_reply,
						path, g_free) == FALSE) {
		rl_printf("Failed to remove device\n");
		g_free(path);
	}
}

static void cmd_remove(const char *arg)
{
	GDBusProxy *proxy;

	if (!arg || !strlen(arg)) {
		rl_printf("Missing device address argument\n");
		return;
	}

	if (check_default_ctrl() == FALSE)
		return;

	if (strcmp(arg, "*") == 0) {
		GList *list;

		for (list = default_ctrl->devices; list;
						list = g_list_next(list)) {
			GDBusProxy *proxy = list->data;

			remove_device(proxy);
		}
		return;
	}

	proxy = find_proxy_by_address(default_ctrl->devices, arg);
	if (!proxy) {
		rl_printf("Device %s not available\n", arg);
		return;
	}

	remove_device(proxy);
}
static void cmd_version(const char *arg)
{
	rl_printf("Version %s\n", VERSION);
}

static void cmd_quit(const char *arg)
{
	g_main_loop_quit(main_loop);
}
static char *generic_generator(const char *text, int state,
					GList *source, const char *property)
{
	static int index, len;
	GList *list;

	if (!state) {
		index = 0;
		len = strlen(text);
	}

	for (list = g_list_nth(source, index); list;
						list = g_list_next(list)) {
		GDBusProxy *proxy = list->data;
		DBusMessageIter iter;
		const char *str;

		index++;

		if (g_dbus_proxy_get_property(proxy, property, &iter) == FALSE)
			continue;

		dbus_message_iter_get_basic(&iter, &str);

		if (!strncmp(str, text, len))
			return strdup(str);
        }

	return NULL;
}

static char *ctrl_generator(const char *text, int state)
{
	static int index = 0;
	static int len = 0;
	GList *list;

	if (!state) {
		index = 0;
		len = strlen(text);
	}

	for (list = g_list_nth(ctrl_list, index); list;
						list = g_list_next(list)) {
		struct adapter *adapter = list->data;
		DBusMessageIter iter;
		const char *str;

		index++;

		if (g_dbus_proxy_get_property(adapter->proxy,
					"Address", &iter) == FALSE)
			continue;

		dbus_message_iter_get_basic(&iter, &str);

		if (!strncmp(str, text, len))
			return strdup(str);
	}

	return NULL;
}

static char *dev_generator(const char *text, int state)
{
	return generic_generator(text, state,
			default_ctrl ? default_ctrl->devices : NULL, "Address");
}
static const struct {
	const char *cmd;
	const char *arg;
	void (*func) (const char *arg);
	const char *desc;
	char * (*gen) (const char *text, int state);
	void (*disp) (char **matches, int num_matches, int max_length);
} cmd_table[] = {
	{ "list",         NULL,       cmd_list, "List available controllers" },
	{ "show",         "[ctrl]",   cmd_show, "Controller information",
							ctrl_generator },
	{ "select",       "<ctrl>",   cmd_select, "Select default controller",
							ctrl_generator },
	{ "devices",      NULL,       cmd_devices, "List available devices" },

	{ "scan",         "<on/off>", cmd_scan, "Scan for devices" },
	{ "info",         "[dev]",    cmd_info, "Device information",
							dev_generator },

	{ "remove",       "<dev>",    cmd_remove, "Remove device",
							dev_generator },

	{ "version",      NULL,       cmd_version, "Display version" },
	{ "quit",         NULL,       cmd_quit, "Quit program" },
	{ "exit",         NULL,       cmd_quit },
	{ "help" },
	{ }
};

static char *cmd_generator(const char *text, int state)
{
	static int index, len;
	const char *cmd;

	if (!state) {
		index = 0;
		len = strlen(text);
	}

	while ((cmd = cmd_table[index].cmd)) {
		index++;

		if (!strncmp(cmd, text, len))
			return strdup(cmd);
	}

	return NULL;
}
static char **cmd_completion(const char *text, int start, int end)
{
	char **matches = NULL;

	if (agent_completion() == TRUE) {
		rl_attempted_completion_over = 1;
		return NULL;
	}

	if (start > 0) {
		int i;

		for (i = 0; cmd_table[i].cmd; i++) {
			if (strncmp(cmd_table[i].cmd,
					rl_line_buffer, start - 1))
				continue;

			if (!cmd_table[i].gen)
				continue;

			rl_completion_display_matches_hook = cmd_table[i].disp;
			matches = rl_completion_matches(text, cmd_table[i].gen);
			break;
		}
	} else {
		rl_completion_display_matches_hook = NULL;
		matches = rl_completion_matches(text, cmd_generator);
	}

	if (!matches)
		rl_attempted_completion_over = 1;

	return matches;
}
static void rl_handler(char *input)
{
	char *cmd, *arg;
	int i;

	if (!input) {
		rl_insert_text("quit");
		rl_redisplay();
		rl_crlf();
		g_main_loop_quit(main_loop);
		return;
	}

	if (!strlen(input))
		goto done;

	if (agent_input(dbus_conn, input) == TRUE)
		goto done;

	add_history(input);

	cmd = strtok_r(input, " ", &arg);
	if (!cmd)
		goto done;

	if (arg) {
		int len = strlen(arg);
		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';
	}

	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;

		if (cmd_table[i].func) {
			cmd_table[i].func(arg);
			goto done;
		}
	}

	if (strcmp(cmd, "help")) {
		printf("Invalid command\n");
		goto done;
	}

	printf("Available commands:\n");

	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("  %s %-*s %s\n", cmd_table[i].cmd,
					(int)(25 - strlen(cmd_table[i].cmd)),
					cmd_table[i].arg ? : "",
					cmd_table[i].desc ? : "");
	}

done:
	free(input);
}
static gboolean signal_handler(GIOChannel *channel, GIOCondition condition,
							gpointer user_data)
{
	static bool terminated = false;
	struct signalfd_siginfo si;
	ssize_t result;
	int fd;

	if (condition & (G_IO_NVAL | G_IO_ERR | G_IO_HUP)) {
		g_main_loop_quit(main_loop);
		return FALSE;
	}

	fd = g_io_channel_unix_get_fd(channel);

	result = read(fd, &si, sizeof(si));
	if (result != sizeof(si))
		return FALSE;

	switch (si.ssi_signo) {
	case SIGINT:
		if (input) {
			rl_replace_line("", 0);
			rl_crlf();
			rl_on_new_line();
			rl_redisplay();
			break;
		}

		/*
		 * If input was not yet setup up that means signal was received
		 * while daemon was not yet running. Since user is not able
		 * to terminate client by CTRL-D or typing exit treat this as
		 * exit and fall through.
		 */
	case SIGTERM:
		if (!terminated) {
			rl_replace_line("", 0);
			rl_crlf();
			g_main_loop_quit(main_loop);
		}

		terminated = true;
		break;
	}

	return TRUE;
}

static guint setup_signalfd(void)
{
	GIOChannel *channel;
	guint source;
	sigset_t mask;
	int fd;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		perror("Failed to set signal mask");
		return 0;
	}

	fd = signalfd(-1, &mask, 0);
	if (fd < 0) {
		perror("Failed to create signal descriptor");
		return 0;
	}

	channel = g_io_channel_unix_new(fd);

	g_io_channel_set_close_on_unref(channel, TRUE);
	g_io_channel_set_encoding(channel, NULL, NULL);
	g_io_channel_set_buffered(channel, FALSE);

	source = g_io_add_watch(channel,
				G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
				signal_handler, NULL);

	g_io_channel_unref(channel);

	return source;
}
static gboolean option_version = FALSE;

static gboolean parse_agent(const char *key, const char *value,
					gpointer user_data, GError **error)
{
	if (value)
		auto_register_agent = g_strdup(value);
	else
		auto_register_agent = g_strdup("");

	return TRUE;
}
static GOptionEntry options[] = {
	{ "version", 'v', 0, G_OPTION_ARG_NONE, &option_version,
				"Show version information and exit" },
	{ "agent", 'a', G_OPTION_FLAG_OPTIONAL_ARG,
				G_OPTION_ARG_CALLBACK, parse_agent,
				"Register agent handler", "CAPABILITY" },
	{ NULL },
};

static void client_ready(GDBusClient *client, void *user_data)
{
	if (!input)
		input = setup_standard_input();
}

int main(int argc, char *argv[])
{
	GOptionContext *context;
	GError *error = NULL;
	GDBusClient *client;
	guint signal;

	context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, options, NULL);

	if (g_option_context_parse(context, &argc, &argv, &error) == FALSE) {
		if (error != NULL) {
			g_printerr("%s\n", error->message);
			g_error_free(error);
		} else
			g_printerr("An unknown error occurred\n");
		exit(1);
	}

	g_option_context_free(context);

	if (option_version == TRUE) {
		printf("%s\n", VERSION);
		exit(0);
	}

	main_loop = g_main_loop_new(NULL, FALSE);
	dbus_conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, NULL, NULL);

	setlinebuf(stdout);
	rl_attempted_completion_function = cmd_completion;

	rl_erase_empty_line = 1;
	rl_callback_handler_install(NULL, rl_handler);

	rl_set_prompt(PROMPT_OFF);
	rl_redisplay();

	signal = setup_signalfd();
	client = g_dbus_client_new(dbus_conn, "org.bluez", "/org/bluez");

	g_dbus_client_set_connect_watch(client, connect_handler, NULL);
	g_dbus_client_set_disconnect_watch(client, disconnect_handler, NULL);
	g_dbus_client_set_signal_watch(client, message_handler, NULL);

	g_dbus_client_set_proxy_handlers(client, proxy_added, proxy_removed,
							property_changed, NULL);

	g_dbus_client_set_ready_watch(client, client_ready, NULL);

	g_main_loop_run(main_loop);

	g_dbus_client_unref(client);
	g_source_remove(signal);
	if (input > 0)
		g_source_remove(input);

	rl_message("");
	rl_callback_handler_remove();

	dbus_connection_unref(dbus_conn);
	g_main_loop_unref(main_loop);

	g_list_free_full(ctrl_list, proxy_leak);

	g_free(auto_register_agent);

	return 0;
}
