/*
 * mainpage.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#ifndef GUI_INC_MAINPAGE_H_
#define GUI_INC_MAINPAGE_H_

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../../libLVGL/lvgl/lvgl.h"
#endif

#if defined(LV_EX_CONF_PATH)
#define __LV_TO_STR_AUX(x) #x
#define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
#include __LV_TO_STR(LV_EX_CONF_PATH)
#undef __LV_TO_STR_AUX
#undef __LV_TO_STR
#elif defined(LV_EX_CONF_INCLUDE_SIMPLE)
#include "lv_ex_conf.h"
#else
#include "../../libLVGL/lv_ex_conf.h"
#endif
#include "stdio.h"


extern lv_style_t style_box;


void ta_event_cb(lv_obj_t * ta, lv_event_t e);
void kb_event_cb(lv_obj_t * _kb, lv_event_t e);
void color_chg_event_cb(lv_obj_t * sw, lv_event_t e);
void table_event_cb(lv_obj_t * table, lv_event_t e);
void lv_demo_widgets(void);
#endif /* GUI_INC_MAINPAGE_H_ */
