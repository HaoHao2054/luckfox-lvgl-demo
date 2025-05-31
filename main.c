#include "lvgl/lvgl.h"
#include "DEV_Config.h"
#include "lv_drivers/display/fbdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (240 * 240)

/*Image declare*/
LV_IMG_DECLARE(Luckfox);  
LV_IMG_DECLARE(cat);  
LV_IMG_DECLARE(galaxy);  
LV_IMG_DECLARE(saint);  

void fbdev_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);

/**
 * A meter with multiple arcs
 */

void lv_show_img(lv_obj_t * img,const lv_img_dsc_t img_dat){
    lv_obj_clean(img);
    lv_img_set_src(img, &img_dat);
    lv_obj_center(img);
}

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 240;
    disp_drv.ver_res    = 240;
    lv_disp_drv_register(&disp_drv);

    /*Initialize pin*/
    DEV_ModuleInit();

    /*Show an image*/
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    lv_obj_t *img = lv_img_create(scr);
    lv_show_img(img,Luckfox);
    lv_obj_center(img);

    /*Create a cursor*/
    lv_obj_t *cursor = lv_img_create(scr);
    lv_img_set_src(cursor, LV_SYMBOL_GPS);
    lv_obj_set_pos(cursor, 70, 120);
    int x=70,y=120,move=0;

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);

        /*Key*/
        if(GET_KEYA == 0){
            lv_show_img(img,Luckfox);
        }
        else if(GET_KEYB == 0){
            lv_show_img(img,cat);
        }
        else if(GET_KEYX == 0){
            lv_show_img(img,galaxy);
        }            
        else if(GET_KEYY == 0){
            lv_show_img(img,saint);
        }

        /*Joystick*/
        if(GET_KEY_UP == 0){
            x += 1;
            if(x > 226)x = 226;
            move =1;
        }
        else if(GET_KEY_DOWN == 0){
            x -= 1;
            if(x < 0)x = 0;
            move =1;
        }
       else if(GET_KEY_LEFT == 0){
            y -= 1;
            if(y < 0)y = 0;
            move =1;
        }            
        else if(GET_KEY_RIGHT == 0){
            y += 1;
            if(y > 224)y = 224;
            move =1;
        }
       else if(GET_KEY_PRESS == 0){
            x = 70;
            y = 120;
            move =1;
        }
        if(move == 1){
            lv_obj_set_pos(cursor, x, y);
            move = 0;
        }
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

