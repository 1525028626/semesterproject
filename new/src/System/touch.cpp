#include "System/touch.h"

static void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

lv_indev_t * indev_touchpad;

void touch_init(void)
{
    static lv_indev_drv_t indev_drv;
    FT6336_Init();
    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}

 
 
/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{   
    uint touchX, touchY;
    FT6336_IRQ(touchX,touchY);
    if(touchX != 0 && touchY != 0) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}
