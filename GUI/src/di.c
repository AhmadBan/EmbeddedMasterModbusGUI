/*
 * discrete_register.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */
#include <di.h>
lv_obj_t * page;
static lv_style_t style_cell4;
static lv_obj_t * diTable;
lv_obj_t * diAddress;
lv_obj_t * diQuantity;
extern modbus_t *mb;
static void diTableInitialize(int start,int count){

	char temp[10]={0};
	uint8_t tab_reg[60]={0};
	int address=0;


		if(count>60){
			count=60;
			lv_textarea_set_text(diQuantity, "60");
		}
		if(diTable!=NULL){
			lv_obj_del(diTable);
			diTable=NULL;
		}


		if(mb!=NULL)
			modbus_read_input_bits(mb, start, count, tab_reg);
		if(diTable==NULL){
			lv_coord_t table_w_max = lv_page_get_width_fit(page);
				lv_page_set_scroll_propagation(page, true);
		diTable = lv_table_create(page, NULL);
			lv_obj_add_style(diTable, LV_TABLE_PART_CELL4, &style_cell4);
			/*Clean the background style of the table because it is placed on a page*/
			lv_obj_clean_style_list(diTable, LV_TABLE_PART_BG);
			lv_obj_set_drag_parent(diTable, true);
			lv_obj_set_event_cb(diTable, table_event_cb);
			lv_table_set_col_cnt(diTable, 6);

			lv_table_set_col_width(diTable, 0,  table_w_max  / 6);
			lv_table_set_col_width(diTable, 1,  table_w_max / 6);
			lv_table_set_col_width(diTable, 2, table_w_max / 6);
			lv_table_set_col_width(diTable, 3,  table_w_max / 6);
			lv_table_set_col_width(diTable, 4, table_w_max / 6);
			lv_table_set_col_width(diTable, 5,  table_w_max / 6);


			lv_table_set_cell_value(diTable, 0, 0, "Address");
			lv_table_set_cell_value(diTable, 0, 1, "Value");
			lv_table_set_cell_value(diTable, 0, 2, "Address");
			lv_table_set_cell_value(diTable, 0, 3, "Value");
			lv_table_set_cell_value(diTable, 0, 4, "Address");
			lv_table_set_cell_value(diTable, 0, 5, "Value");
		}
		address=start;
		for(int i=0;i<count;i++){

				sprintf(temp,"%d",address++);
				lv_table_set_cell_value(diTable,i/3+1, (i*2)%6, temp);

				sprintf(temp,"%d",tab_reg[i]);
				lv_table_set_cell_value(diTable,i/3+1, (i*2)%6+1, temp);

			}


}
static void diReadBtnEvent_handler(lv_obj_t * obj, lv_event_t event)
{
	const char *temp;
	int address,quantity;

	if(event == LV_EVENT_CLICKED) {

		temp=lv_textarea_get_text(diAddress);
		sscanf(temp,"%d",&address);
		temp=lv_textarea_get_text(diQuantity);
		sscanf(temp,"%d",&quantity);
		//modbus_read_registers(mb, address, quantity, tab_reg);
		diTableInitialize(address,quantity);


	}
	else if(event == LV_EVENT_VALUE_CHANGED) {
		printf("Toggled\n");

	}
}




void discreteInput_create(lv_obj_t * parent){
	const char * temp;
	int address,quantity;
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_h = lv_page_get_height_grid(parent, 1,1);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);

	lv_obj_t * diCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(diCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(diCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(diCont, true);

	lv_cont_set_fit2(diCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(diCont, grid_w);
	lv_obj_set_style_local_value_str(diCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Command");

	lv_obj_t * functionDd = lv_dropdown_create(diCont, NULL);
	lv_dropdown_set_options(functionDd, "02: Read Discrete Input");
	lv_obj_set_width(functionDd, grid_w-20);

	diAddress = lv_textarea_create(diCont, NULL);
	lv_textarea_set_placeholder_text(diAddress, "Address");
	lv_textarea_set_text(diAddress, "0");
	lv_textarea_set_one_line(diAddress, true);
	lv_textarea_set_cursor_hidden(diAddress, true);
	lv_obj_set_width(diAddress, grid_w/2-20);
	lv_obj_set_event_cb(diAddress, ta_event_cb);

	diQuantity = lv_textarea_create(diCont, NULL);
	lv_textarea_set_placeholder_text(diQuantity, "Quantity");
	lv_textarea_set_text(diQuantity, "60");
	lv_textarea_set_one_line(diQuantity, true);
	lv_textarea_set_cursor_hidden(diQuantity, true);
	lv_obj_set_width(diQuantity, grid_w/2-20);
	lv_obj_set_event_cb(diQuantity, ta_event_cb);

	lv_obj_t * diReadBtn = lv_btn_create(diCont, NULL);
	lv_btn_set_fit2(diReadBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(diReadBtn, lv_obj_get_width_grid(diCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t * label = lv_label_create(diReadBtn, NULL);
	lv_label_set_text(label ,"Read");
	lv_obj_set_event_cb(diReadBtn, diReadBtnEvent_handler);
	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent,  1, 1);

	lv_style_init(&style_cell4);
	lv_style_set_bg_opa(&style_cell4, LV_STATE_DEFAULT, LV_OPA_50);
	lv_style_set_bg_color(&style_cell4, LV_STATE_DEFAULT, LV_COLOR_GRAY);

	page = lv_page_create(parent ,NULL);
	lv_obj_set_size(page, grid_w, grid_h);


	temp=lv_textarea_get_text(diAddress);
	sscanf(temp,"%d",&address);
	temp=lv_textarea_get_text(diQuantity);
	sscanf(temp,"%d",&quantity);
	diTableInitialize(address,quantity);
}
