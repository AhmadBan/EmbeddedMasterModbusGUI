/*
 * holdingregister.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */


#include <ai.h>

static lv_obj_t * page;
static lv_style_t style_cell4;
static lv_obj_t * aiTable;
static lv_obj_t * aiAddress;
static lv_obj_t * aiQuantity;
extern modbus_t *mb;
static void aiTableInitialize(int start,int count){

	char temp[10]={0};
	int16_t tab_reg[60]={0};
	int address=0;


		if(count>60){
			count=60;
			lv_textarea_set_text(aiQuantity, "60");
		}
		if(aiTable!=NULL){
			lv_obj_del(aiTable);
			aiTable=NULL;
		}


		if(mb!=NULL)
			modbus_read_input_registers(mb, start, count, tab_reg);
		if(aiTable==NULL){
			lv_coord_t table_w_max = lv_page_get_width_fit(page);
				lv_page_set_scroll_propagation(page, true);
		aiTable = lv_table_create(page, NULL);
			lv_obj_add_style(aiTable, LV_TABLE_PART_CELL4, &style_cell4);
			/*Clean the background style of the table because it is placed on a page*/
			lv_obj_clean_style_list(aiTable, LV_TABLE_PART_BG);
			lv_obj_set_drag_parent(aiTable, true);
			lv_obj_set_event_cb(aiTable, table_event_cb);
			lv_table_set_col_cnt(aiTable, 6);

			lv_table_set_col_width(aiTable, 0,  table_w_max  / 6);
			lv_table_set_col_width(aiTable, 1,  table_w_max / 6);
			lv_table_set_col_width(aiTable, 2, table_w_max / 6);
			lv_table_set_col_width(aiTable, 3,  table_w_max / 6);
			lv_table_set_col_width(aiTable, 4, table_w_max / 6);
			lv_table_set_col_width(aiTable, 5,  table_w_max / 6);


			lv_table_set_cell_value(aiTable, 0, 0, "Address");
			lv_table_set_cell_value(aiTable, 0, 1, "Value");
			lv_table_set_cell_value(aiTable, 0, 2, "Address");
			lv_table_set_cell_value(aiTable, 0, 3, "Value");
			lv_table_set_cell_value(aiTable, 0, 4, "Address");
			lv_table_set_cell_value(aiTable, 0, 5, "Value");
		}
		address=start;
		for(int i=0;i<count;i++){

				sprintf(temp,"%d",address++);
				lv_table_set_cell_value(aiTable,i/3+1, (i*2)%6, temp);

				sprintf(temp,"%d",tab_reg[i]);
				lv_table_set_cell_value(aiTable,i/3+1, (i*2)%6+1, temp);

			}


}
static void aiReadBtnEvent_handler(lv_obj_t * obj, lv_event_t event)
{
	const char *temp;
	int address,quantity;

	if(event == LV_EVENT_CLICKED) {

		temp=lv_textarea_get_text(aiAddress);
		sscanf(temp,"%d",&address);
		temp=lv_textarea_get_text(aiQuantity);
		sscanf(temp,"%d",&quantity);
		//modbus_read_registers(mb, address, quantity, tab_reg);
		aiTableInitialize(address,quantity);


	}
	else if(event == LV_EVENT_VALUE_CHANGED) {
		printf("Toggled\n");

	}
}




void inputRegister_create(lv_obj_t * parent){

	const char * temp;
	int address,quantity;

	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_h = lv_page_get_height_grid(parent, 1,1);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);

	lv_obj_t * aiCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(aiCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(aiCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(aiCont, true);

	lv_cont_set_fit2(aiCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(aiCont, grid_w);
	lv_obj_set_style_local_value_str(aiCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Command");

	lv_obj_t * functionCoil = lv_dropdown_create(aiCont, NULL);
	lv_dropdown_set_options(functionCoil, "04: Read Input Registers");
	lv_obj_set_width(functionCoil, grid_w-20);

	aiAddress = lv_textarea_create(aiCont, NULL);
	lv_textarea_set_placeholder_text(aiAddress, "Address");
	lv_textarea_set_text(aiAddress, "0");
	lv_textarea_set_one_line(aiAddress, true);
	lv_textarea_set_cursor_hidden(aiAddress, true);
	lv_obj_set_width(aiAddress, grid_w/2-20);
	lv_obj_set_event_cb(aiAddress, ta_event_cb);

	aiQuantity = lv_textarea_create(aiCont, NULL);
	lv_textarea_set_placeholder_text(aiQuantity, "Quantity");
	lv_textarea_set_text(aiQuantity, "60");
	lv_textarea_set_one_line(aiQuantity, true);
	lv_textarea_set_cursor_hidden(aiQuantity, true);
	lv_obj_set_width(aiQuantity, grid_w/2-20);
	lv_obj_set_event_cb(aiQuantity, ta_event_cb);


	lv_obj_t * aiReadBtn = lv_btn_create(aiCont, NULL);
	lv_btn_set_fit2(aiReadBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(aiReadBtn, lv_obj_get_width_grid(aiCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t * label = lv_label_create(aiReadBtn, NULL);
	lv_obj_set_event_cb(aiReadBtn, aiReadBtnEvent_handler);
	lv_label_set_text(label ,"OK");

	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent,  1, 1);



	lv_style_init(&style_cell4);
	lv_style_set_bg_opa(&style_cell4, LV_STATE_DEFAULT, LV_OPA_50);
	lv_style_set_bg_color(&style_cell4, LV_STATE_DEFAULT, LV_COLOR_GRAY);

	page = lv_page_create(parent ,NULL);
	lv_obj_set_size(page, grid_w, grid_h);
	lv_coord_t table_w_max = lv_page_get_width_fit(page);
	lv_page_set_scroll_propagation(page, true);

	aiTable = lv_table_create(page, NULL);
	lv_obj_add_style(aiTable, LV_TABLE_PART_CELL4, &style_cell4);
	/*Clean the background style of the table because it is placed on a page*/
	lv_obj_clean_style_list(aiTable, LV_TABLE_PART_BG);
	lv_obj_set_drag_parent(aiTable, true);
	lv_obj_set_event_cb(aiTable, table_event_cb);
	lv_table_set_col_cnt(aiTable, 6);


	temp=lv_textarea_get_text(aiAddress);
	sscanf(temp,"%d",&address);
	temp=lv_textarea_get_text(aiQuantity);
	sscanf(temp,"%d",&quantity);
	aiTableInitialize(address,quantity);


}
