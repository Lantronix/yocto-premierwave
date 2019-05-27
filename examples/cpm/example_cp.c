#include "cp_userapi.h"

/* All 13 pins */
#define CP1 1
#define CP2 2
#define CP3 3
#define CP4 4
#define CP5 5
#define CP6 6
#define CP7 7
#define CP8 8
#define CP9 9
#define CP10 10
#define CP11 11
#define CP12 12
#define CP13 13

int main(void)
{
    int ret = -1;
    /* init the config and it is must before controlling the CP */
    ret = cp_config_init();
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* Set Direction of CP for output */
    ret = cp_set_direction_out(CP1);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* After setting direction to make it High*/
    ret = cp_set_output_high(CP1);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* Getting the CP Value when it is an Input or Output */
    int p = -1;
    ret = cp_get_pin_value(CP1, &p);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* After setting direction to make it Low*/
    ret = cp_set_output_low(CP1);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* Set Direction of CP for input */
    ret = cp_set_direction_in(CP2);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* To set active low */
    ret = cp_set_active_low(CP2);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }

    /* To unset active low */
    ret = cp_set_active_high(CP2);
    if (ret < 0)
    {
        printf("Not Successfull\n");
        return ret;
    }
    return 0;
}

