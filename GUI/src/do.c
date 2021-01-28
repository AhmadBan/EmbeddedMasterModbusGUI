/*
 * coil.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#include <do.h>
extern lv_style_t style_box;
lv_obj_t * coilAddress;
lv_obj_t * coilQuantity;
lv_obj_t * coilReadBtn;
lv_obj_t * coilCont;
lv_obj_t * btn[20];
lv_obj_t * h;

extern modbus_t *mb;
static void coilOkBtnEvent_handler(lv_obj_t * obj, lv_event_t event)
{
	const char *temp;
	int address,quantity;
	uint8_t tab_reg[20]={0};
	if(event == LV_EVENT_CLICKED) {
		lv_obj_t * child=lv_obj_get_child(obj, NULL);
		if(strcmp(lv_label_get_text(child),"Write All")==0){
			temp=lv_textarea_get_text(coilAddress);
			sscanf(temp,"%d",&address);
			temp=lv_textarea_get_text(coilQuantity);
			sscanf(temp,"%d",&quantity);
			for(int i=0;i<quantity;i++){
				if(lv_obj_get_state(btn[i], LV_BTN_PART_MAIN) & LV_STATE_CHECKED)
					tab_reg[i]=0xff;
				else
					tab_reg[i]=0x00;
			}
			modbus_write_bits(mb, address, quantity, tab_reg);

		}else {temp=lv_textarea_get_text(coilAddress);
			sscanf(temp,"%d",&address);
			temp=lv_textarea_get_text(coilQuantity);
			sscanf(temp,"%d",&quantity);
			//modbus_read_registers(mb, address, quantity, tab_reg);
			initiateCoils(address,quantity);
		}


	}
	else if(event == LV_EVENT_VALUE_CHANGED) {
		printf("Toggled\n");

	}
}

static void btnEvent_Handler(lv_obj_t * obj, lv_event_t event){
	if(event == LV_EVENT_CLICKED) {
		int num,state;
		lv_obj_t * child=lv_obj_get_child(obj, NULL);
		char* temp= lv_label_get_text(child);
		sscanf(temp,"coil%d",&num);
		if(lv_obj_get_state(obj, LV_BTN_PART_MAIN) & LV_STATE_CHECKED)
			state=1;
		else
			state=0;
		modbus_write_bit(mb,num, state);



	}
}

void enableSingleWrite(int status){
	for(int i=0;i<20;i++){
			if(btn[i]!=NULL){
				if(status)
					lv_obj_set_event_cb(btn[i], btnEvent_Handler);
				else
					lv_obj_set_event_cb(btn[i], NULL);
			}
		}

}

void initiateCoils(int start,int count){
	char temp[10]={0};
	uint8_t tab_reg[20]={0};
	lv_obj_t * label;
	if(count>20){
		count=20;
		lv_textarea_set_text(coilQuantity, "20");
	}


	for(int i=0;i<20;i++){
		if(btn[i]!=NULL){
			lv_obj_del(btn[i]);
			btn[i]=NULL;
		}
	}
	if(mb!=NULL)
		modbus_read_bits(mb, start, count, tab_reg);
	for(int i=start;i<count+start;i++){
		btn[i-start]= lv_btn_create(h, NULL);//lv_checkbox_create(h, NULL);

		lv_obj_align(btn[i-start], NULL, LV_ALIGN_CENTER, 0, 40);
		lv_btn_set_checkable(btn[i-start], true);

		lv_btn_set_fit2(btn[i-start], LV_FIT_NONE, LV_FIT_TIGHT);
		sprintf(temp,"coil%02d",i);
		label = lv_label_create(btn[i-start], NULL);
		lv_label_set_text(label, temp);
		if(tab_reg[i-start]!=0)
			lv_btn_toggle(btn[i-start]);


	}

}
 static void createButton(char* text){
	 lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	 coilReadBtn = lv_btn_create(coilCont, NULL);
	 lv_btn_set_fit2(coilReadBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	 lv_obj_set_width(coilReadBtn, lv_obj_get_width_grid(coilCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));

	 lv_obj_set_event_cb(coilReadBtn, coilOkBtnEvent_handler);
	 lv_obj_t * okLabel = lv_label_create(coilReadBtn, NULL);
	 lv_label_set_text(okLabel ,text);
 }

static void coilFuncEvent_handler(lv_obj_t * obj, lv_event_t event)
{


	if(event == LV_EVENT_VALUE_CHANGED) {
		char buf[32];
		lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
		if(coilReadBtn!=NULL){
			lv_obj_del(coilReadBtn);
			coilReadBtn=NULL;
		}

		if(strcmp(buf,"01: Read Coils" )==0){

			createButton("Read");
			enableSingleWrite(0);

		}
		if(strcmp(buf,"05: Write Single Coil" )==0)
		{
			enableSingleWrite(1);
		}
		if(strcmp(buf,"15: Write Multiple Coils" )==0){
			createButton("Update");
			createButton("Write All");
			enableSingleWrite(0);
		}

	}
}


void coil_create(lv_obj_t * parent){
	const char *temp;
	int address=0,quantity=0;

	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);

	coilCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(coilCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(coilCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(coilCont, true);

	lv_cont_set_fit2(coilCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(coilCont, grid_w);
	lv_obj_set_style_local_value_str(coilCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Command");

	lv_obj_t * functionCoil = lv_dropdown_create(coilCont, NULL);
	lv_dropdown_set_options(functionCoil, "01: Read Coils\n05: Write Single Coil\n15: Write Multiple Coils");
	lv_obj_set_width(functionCoil, grid_w-20);
	lv_obj_set_event_cb(functionCoil, coilFuncEvent_handler);


	coilAddress = lv_textarea_create(coilCont, NULL);
	lv_textarea_set_placeholder_text(coilAddress, "Address");
	lv_textarea_set_text(coilAddress, "0");
	lv_textarea_set_one_line(coilAddress, true);
	lv_textarea_set_cursor_hidden(coilAddress, true);
	lv_obj_set_width(coilAddress, grid_w/2-20);
	lv_obj_set_event_cb(coilAddress, ta_event_cb);

	coilQuantity = lv_textarea_create(coilCont, NULL);
	lv_textarea_set_placeholder_text(coilQuantity, "Quantity");
	lv_textarea_set_text(coilQuantity, "20");
	lv_textarea_set_one_line(coilQuantity, true);
	lv_textarea_set_cursor_hidden(coilQuantity, true);
	lv_obj_set_width(coilQuantity, grid_w/2-20);
	lv_obj_set_event_cb(coilQuantity, ta_event_cb);


	coilReadBtn = lv_btn_create(coilCont, NULL);
	lv_btn_set_fit2(coilReadBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(coilReadBtn, lv_obj_get_width_grid(coilCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));

	lv_obj_set_event_cb(coilReadBtn, coilOkBtnEvent_handler);
	lv_obj_t * okLabel = lv_label_create(coilReadBtn, NULL);
	lv_label_set_text(okLabel ,"Read");



	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent,  1, 1);

	h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(h, true);

	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Coils");

	lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(h, grid_w);
	grid_w = lv_page_get_width_grid(parent,  6, 1);
	temp=lv_textarea_get_text(coilAddress);
	sscanf(temp,"%d",&address);
	temp=lv_textarea_get_text(coilQuantity);
	sscanf(temp,"%d",&quantity);

	initiateCoils(address,quantity);
}

