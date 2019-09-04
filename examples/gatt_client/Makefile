all:
	$(CC) -Wno-poison-system-directories example_ltrx_gatttool.c -o example_ltrx_gatttool -I/usr/include/glib-2.0/ -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include/ -I/usr/include/dbus-1.0/ -I./include/ -L. -lltrx-gattclient -lglib-2.0 -ldbus-1 -lreadline

clean:
	rm example_ltrx_gatttool
