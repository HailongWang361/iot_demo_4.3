/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#if defined(_MSC_VER)
#else
#include "stdio.h"
#endif

#include "sensor_demo.h"


#include "gdi_font_engine.h"
#include "gdi.h"
//#include "graphic_interface.h"
#include "main_screen.h"
#include "bt_gap.h"
#include "sensor_demo.h"
#include "mt25x3_hdk_lcd.h"
//add by chen
#include "stdint.h"
#include "stdbool.h"
#include <string.h>
#include "stdlib.h"
#include "memory_attribute.h"
#include "bsp_lcd.h"
#include "mt25x3_hdk_backlight.h"
#include "fota_demo.h"

#include "syslog.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "custom_image_data_resource.h"
#include "custom_resource_def.h"

#define LEFT_GAP 5
#define ITEM_HEIGHT 25

#define RESIZE_RATE LCD_CURR_HEIGHT/240
#define DEMO_ITEM_NAME_MAX_LEN 50

#define IMG_UPDATE_HEIGHT 49
#define IMG_UPDATE_WIDTH 320  /* Set to the MAX LCD size for dynamic adjust the LCD size*/

#define LCD_WIDTH   (240)
#define LCD_HEIGHT (240)


typedef enum{
    FOTA_APP_LCD_TYPE_HEIGHT,
    FOTA_APP_LCD_TYPE_WIDTH
}fota_app_lcd_type_t;

static struct {
  int32_t fota_title_x;
  int32_t fota_title_y;
  int32_t hr_title_x;
  int32_t hr_title_y;
  int32_t hrv_title_x;
  int32_t hrv_title_y;
  int32_t hrv_value_x;
  int32_t hrv_value_y;
  int32_t bt_name_x;
  int32_t bt_name_y;
  int32_t bt_status_x;
  int32_t bt_status_y;
  int32_t back_x1;
  int32_t back_y1;
  int32_t back_x2;
  int32_t back_y2;
  int32_t start_item;
  int32_t curr_item_num;
  int32_t one_screen_item_num;
  int32_t total_item_num;
  int32_t focus_point_index;
  gdi_color_t bg_color;
  gdi_color_t font_color;
  uint32_t width;
  uint32_t height;
} fota_screen_cntx;


uint32_t g_fota_index_color_table[16] = 
{
	0x0,
	0xFFFF,
	0x7E0,
	0x1F,
	0x4208,
	0x8410,
	0xFFFF,
	0xFF00,
	0x0,
	0xF800,
	0x7E0,
	0x1F,
	0x4208,
	0x8410,
	0xFFFF,
	0xFF00
};


static hal_display_lcd_roi_output_t lcd_para_fota;
static hal_display_lcd_layer_input_t lcm_para_fota;

extern gdi_resource_custom_image_t	gdi_resource_custom_image_names[];
extern gdi_resource_custom_image_map_t gdi_resource_custom_image_id_map[];

void show_fota_screen(void);

void fota_event_handler(message_id_enum event_id, int32_t param1, void* param2)
{

}
static void fota_screen_cntx_init()
{
    if ((fota_screen_cntx.height == 0) && (fota_screen_cntx.width==0)) {

        BSP_LCD_GetParam(LCM_IOCTRL_QUERY__LCM_HEIGHT, &(fota_screen_cntx.height));
        BSP_LCD_GetParam(LCM_IOCTRL_QUERY__LCM_WIDTH, &(fota_screen_cntx.width));
//        LOG_I("width(%d), height(%d).\r\n", fota_screen_cntx.width, fota_screen_cntx.height);

        fota_screen_cntx.bg_color = 0;
        fota_screen_cntx.font_color = 0xFFFF;
        /* first */
        fota_screen_cntx.fota_title_x = 5;
        fota_screen_cntx.fota_title_y = 6;

        /* middle */
        fota_screen_cntx.hr_title_x = LEFT_GAP;
        fota_screen_cntx.hr_title_y = ITEM_HEIGHT*2 + fota_screen_cntx.fota_title_y;
        fota_screen_cntx.hrv_title_x = LEFT_GAP;
        fota_screen_cntx.hrv_title_y = ITEM_HEIGHT + fota_screen_cntx.hr_title_y;
        fota_screen_cntx.hrv_value_x = LEFT_GAP;
        fota_screen_cntx.hrv_value_y = ITEM_HEIGHT + fota_screen_cntx.hrv_title_y;

        /* end */
        fota_screen_cntx.bt_name_x = LEFT_GAP;
        fota_screen_cntx.bt_name_y = fota_screen_cntx.height - 6 - (ITEM_HEIGHT*3);
        fota_screen_cntx.bt_status_x = LEFT_GAP;
        fota_screen_cntx.bt_status_y = ITEM_HEIGHT*2 + fota_screen_cntx.bt_name_y;
        fota_screen_cntx.back_x1 = fota_screen_cntx.width-80;
        fota_screen_cntx.back_y1 = ITEM_HEIGHT*2 + fota_screen_cntx.bt_name_y;
        fota_screen_cntx.back_x2 = fota_screen_cntx.width-1;
        fota_screen_cntx.back_y2 = fota_screen_cntx.height-1;

		fota_screen_cntx.focus_point_index = 0;
		fota_screen_cntx.start_item = 0;
		fota_screen_cntx.one_screen_item_num = 2;
		fota_screen_cntx.total_item_num = 2;
		fota_screen_cntx.curr_item_num = 2;
    }

}

static uint32_t fota_app_get_lcd_size(fota_app_lcd_type_t type)
{
    uint32_t height, width;
    if (type == FOTA_APP_LCD_TYPE_WIDTH) {
        BSP_LCD_GetParam(LCM_IOCTRL_QUERY__LCM_WIDTH, &width);
        //LOG_I(common, "width = %d", width);
        return width;
    } else if (type == FOTA_APP_LCD_TYPE_HEIGHT) {
        BSP_LCD_GetParam(LCM_IOCTRL_QUERY__LCM_HEIGHT, &height);
        //LOG_I(common, "height = %d", height);
        return height;
    } else {
        LOG_E(common, "wrongly lcd type");
        return 0;
    }
   
}


static void fota_app_show_image(void)
{
    uint8_t *img_ptr;
    uint32_t lcd_height = fota_app_get_lcd_size(FOTA_APP_LCD_TYPE_HEIGHT);
    uint32_t lcd_width = fota_app_get_lcd_size(FOTA_APP_LCD_TYPE_WIDTH);
    //LOG_I(common, "app task to draw screen %d", g_wf_is_show_screen);
    

    	  lcd_para_fota.target_start_x = 0;
    	  lcd_para_fota.target_start_y = (lcd_height - IMG_UPDATE_HEIGHT)/2;
    	  lcd_para_fota.target_end_x = lcd_width - 1;
    	  lcd_para_fota.target_end_y = (lcd_height - IMG_UPDATE_HEIGHT)/2+IMG_UPDATE_HEIGHT-1;
    	  lcd_para_fota.roi_offset_x = 0;
    	  lcd_para_fota.roi_offset_y = 0;
          lcd_para_fota.main_lcd_output = LCM_16BIT_16_BPP_RGB565_1;

    	  BSP_LCD_ConfigROI(&lcd_para_fota);
          BSP_LCD_set_index_color_table(g_fota_index_color_table);
    	  lcm_para_fota.source_key_flag= 0;
    	  lcm_para_fota.alpha_flag= 0;
    	  lcm_para_fota.color_format= HAL_DISPLAY_LCD_LAYER_COLOR_1BIT_INDEX;
    	  lcm_para_fota.alpha = 0;
    	  lcm_para_fota.rotate = HAL_DISPLAY_LCD_LAYER_ROTATE_0;
    	  lcm_para_fota.layer_enable= 0;
    	  lcm_para_fota.pitch = lcd_width/8;
    	  lcm_para_fota.row_size = IMG_UPDATE_HEIGHT;
    	  lcm_para_fota.column_size = lcd_width;
    	  lcm_para_fota.window_x_offset = 0;
    	  //lcm_para.window_y_offset = (240 - IMG_UPDATE_HEIGHT)/2;
          lcm_para_fota.window_y_offset = 0;

//          memset(g_wf_time_update_area_img, 0x00, IMG_UPDATE_HEIGHT * lcd_width/8);
//          img_ptr = wf_app_get_time_img_buffer(curr_time);
    	  lcm_para_fota.buffer_address = (uint32_t)((uint8_t *)(gdi_resource_custom_image_names[18].image));
		  LOG_I(common, "fota screen draw screen %d", lcm_para_fota.buffer_address);
    	  BSP_LCD_ConfigLayer(&lcm_para_fota);
    	  BSP_LCD_UpdateScreen(lcd_para_fota.target_start_x, lcd_para_fota.target_start_y, lcd_para_fota.target_end_x, lcd_para_fota.target_end_y);
    
}

static void fota_show_test_image(void)
{
	static uint8_t layer_buffer[LCD_WIDTH * LCD_HEIGHT * 2];
	gdi_init(LCD_WIDTH, LCD_HEIGHT, GDI_COLOR_FORMAT_16, layer_buffer);
	gdi_draw_filled_rectangle(0, 0, LCD_WIDTH, LCD_HEIGHT, gdi_get_color_from_argb(0, 0, 0, 0)); // Clear the screen to black.
	gdi_draw_rectangle(50, 50, 100, 100, gdi_get_color_from_argb(0, 255, 0, 0)); // Draw a red rectangle.
	gdi_draw_rectangle(100, 100, 150, 150, gdi_get_color_from_argb(0, 0, 255, 0));
	gdi_draw_rectangle(150, 150, 200, 200, gdi_get_color_from_argb(0, 0, 0, 255));
//	gdi_image_draw_by_id(10, 10, image_id);
//	gdi_image_draw_by_id(10, 10, 47);
//	gdi_image_draw_by_id(50, 50, gdi_resource_custom_image_id_map[46].image_number);
//	gdi_image_draw_by_id(100, 100, gdi_resource_custom_image_id_map[47].image_number);
//	gdi_image_draw_by_id(150, 150, gdi_resource_custom_image_id_map[8].image_number);
//	gdi_image_draw_by_id(200, 200, gdi_resource_custom_image_id_map[4].image_number);

	gdi_lcd_update_screen(0, 0, LCD_WIDTH, LCD_HEIGHT);

}
static uint8_t* fota_convert_string_to_wstring(char* string)
{
    static uint8_t wstring[50];
    int32_t index = 0;
    if (!string) {
        return NULL;
    }
    while (*string) {
        wstring[index] = *((uint8_t*)string);
        wstring[index + 1] = 0;
        string++;
        index+=2;
    }
    return wstring;
}

void show_fota_screen(void)
{
//	int32_t index = sensor_screen_cntx.start_item;
 //   int32_t num = sensor_screen_cntx.curr_item_num;
    int32_t x,y;

	x = 40 * RESIZE_RATE;
	y = 50 * RESIZE_RATE;
/*
	fota_screen_cntx_init();
	
	gdi_font_engine_display_string_info_t fota_string_info = {0};
    gdi_draw_filled_rectangle(0,0,fota_screen_cntx.width-1,fota_screen_cntx.height-1, fota_screen_cntx.bg_color);

    gdi_font_engine_size_t font = GDI_FONT_ENGINE_FONT_MEDIUM;
    gdi_font_engine_color_t text_color = {0, 255, 255, 255};//white color

    gdi_font_engine_set_font_size(font);
    gdi_font_engine_set_text_color(text_color);

    fota_string_info.baseline_height = -1;
    fota_string_info.x = fota_screen_cntx.fota_title_x;
    fota_string_info.y = fota_screen_cntx.fota_title_y;
    fota_string_info.string = fota_convert_string_to_wstring("FOTA..");
    fota_string_info.length = strlen("FOTA..");
    gdi_font_engine_display_string(&fota_string_info);

	gdi_lcd_update_screen(0,0,fota_screen_cntx.width-1,fota_screen_cntx.height-1);
	LOG_I(common, "fota_app_show_image");
*/
//	fota_app_show_image();
	fota_show_test_image();

}
