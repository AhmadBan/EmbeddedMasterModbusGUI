/*
 * mainpage.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#include <ai.h>
#include <ao.h>
#include <di.h>
#include <do.h>
#include "mainpage.h"
#include "connection.h"

#if LV_USE_THEME_MATERIAL
void color_chg_event_cb(lv_obj_t * sw, lv_event_t e);
#endif




lv_style_t style_box;
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;
static lv_obj_t * t3;
static lv_obj_t * t4;
static lv_obj_t * t5;
lv_obj_t * kb;

void mainGui(void)
{
	tv = lv_tabview_create(lv_scr_act(), NULL);
#if LV_USE_THEME_MATERIAL
	if(LV_THEME_DEFAULT_INIT == lv_theme_material_init) {
		lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
		if(disp_size >= LV_DISP_SIZE_MEDIUM) {
			lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES /4);
			lv_obj_t * sw = lv_switch_create(lv_scr_act(), NULL);
			if(lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
				lv_switch_on(sw, LV_ANIM_OFF);
			lv_obj_set_event_cb(sw, color_chg_event_cb);
			lv_obj_set_pos(sw, LV_DPX(10), LV_DPX(10));
			lv_obj_set_style_local_value_str(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
			lv_obj_set_style_local_value_align(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_MID);
			lv_obj_set_style_local_value_ofs_x(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI/35);
		}
	}
#endif
	t1 = lv_tabview_add_tab(tv, "Connect");
	t2 = lv_tabview_add_tab(tv, "DO");
	t3 = lv_tabview_add_tab(tv, "DI");
	t4 = lv_tabview_add_tab(tv, "AI");
	t5 = lv_tabview_add_tab(tv, "AO");

	lv_style_init(&style_box);
	lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_MID);
	lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, - LV_DPX(10));
	lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));
	connection_create(t1);
	coil_create(t2);
	discreteInput_create(t3);
	holdingRegister_create(t5);
	inputRegister_create(t4);


#if LV_DEMO_WIDGETS_SLIDESHOW
	lv_task_create(tab_changer_task_cb, 8000, LV_TASK_PRIO_LOW, NULL);
#endif
}


void kb_event_cb(lv_obj_t * _kb, lv_event_t e)
{
	lv_keyboard_def_event_cb(kb, e);

	if(e == LV_EVENT_CANCEL) {
		if(kb) {
			lv_obj_set_height(tv, LV_VER_RES);
			lv_obj_del(kb);
			kb = NULL;
		}
	}
}

void ta_event_cb(lv_obj_t * ta, lv_event_t e)
{

	if(e == LV_EVENT_RELEASED) {
		if(kb == NULL) {


			/* Create a keyboard*/
			kb = lv_keyboard_create(lv_scr_act(), NULL);
			lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUM);
			lv_obj_set_event_cb(kb, kb_event_cb);

			lv_indev_wait_release(lv_indev_get_act());
		}

		lv_textarea_set_cursor_hidden(ta, false);
		lv_page_focus(t1, lv_textarea_get_label(ta), LV_ANIM_ON);
		lv_keyboard_set_textarea(kb, ta);
	} else if(e == LV_EVENT_DEFOCUSED) {
		lv_textarea_set_cursor_hidden(ta, true);
		lv_event_send(kb, LV_EVENT_CANCEL, NULL);

	}
}
void table_event_cb(lv_obj_t * table, lv_event_t e)
{
	if(e == LV_EVENT_CLICKED) {
		uint16_t row;
		uint16_t col;
		lv_res_t r = lv_table_get_pressed_cell(table, &row, &col);
		if(r == LV_RES_OK && col && row) {  /*Skip the first row and col*/
			if(lv_table_get_cell_type(table, row, col) == 1) {
				lv_table_set_cell_type(table, row, col, 4);
			} else {
				lv_table_set_cell_type(table, row, col, 1);
			}
		}
	}
}


#if LV_USE_THEME_MATERIAL
void color_chg_event_cb(lv_obj_t * sw, lv_event_t e)
{
	if(LV_THEME_DEFAULT_INIT != lv_theme_material_init) return;
	if(e == LV_EVENT_VALUE_CHANGED) {
		uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
		if(lv_switch_get_state(sw)) flag = LV_THEME_MATERIAL_FLAG_DARK;

		LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
				flag,
				lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());
	}
}
#endif
