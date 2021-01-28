/*
 * inputRegister.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */
#include <ao.h>
extern lv_style_t style_box;
static lv_obj_t * aoAddress;
static lv_obj_t * aoQuantity;
static lv_obj_t * aoRWBtn,*aoUpdateBtn;
static lv_obj_t * aoCont;
lv_obj_t * regCon[20];
lv_obj_t * textAreaCont;
lv_obj_t * okLabel;
lv_obj_t * functionDd;
lv_obj_t * irTa[20],* irLbl[20];
extern lv_obj_t * kb;
extern modbus_t *mb;
static lv_obj_t* createButton(char* text);
static void aoRWBtnEvent_handler(lv_obj_t * obj, lv_event_t event)
{
	const char *temp;
	int address,quantity;
	uint16_t tab_reg[20]={0};
	if(event == LV_EVENT_CLICKED) {
		lv_obj_t * child=lv_obj_get_child(obj, NULL);
		if(strcmp(lv_label_get_text(child),"Write All")==0){
			temp=lv_textarea_get_text(aoAddress);
			sscanf(temp,"%d",&address);
			temp=lv_textarea_get_text(aoQuantity);
			sscanf(temp,"%d",&quantity);
			for(int i=0;i<quantity;i++){
				temp=lv_textarea_get_text(irTa[i]);
				sscanf(temp,"%d",&tab_reg[i]);
			}
			modbus_write_registers(mb, address, quantity, tab_reg);

		}else {
			char buf[32];
			lv_dropdown_get_selected_str(functionDd, buf, sizeof(buf));
			temp=lv_textarea_get_text(aoAddress);
			sscanf(temp,"%d",&address);
			temp=lv_textarea_get_text(aoQuantity);
			sscanf(temp,"%d",&quantity);
			initiateAo(address,quantity);
			if(strcmp(buf,"06: Write Single register" )==0)
			{

				aoEnableSingleWrite(1);
			}
			if(strcmp(buf,"16: Write Multiple registers" )==0){

				aoEnableSingleWrite(2);
			}

		}


	}
	else if(event == LV_EVENT_VALUE_CHANGED) {
		printf("Toggled\n");

	}
}

static void taEvent_Handler(lv_obj_t * ta, lv_event_t e){
	if(e == LV_EVENT_RELEASED) {
		if(kb == NULL) {


			/* Create a keyboard*/
			kb = lv_keyboard_create(lv_scr_act(), NULL);
			lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUM);
			lv_obj_set_event_cb(kb, kb_event_cb);

			lv_indev_wait_release(lv_indev_get_act());
		}

		lv_textarea_set_cursor_hidden(ta, false);

		lv_keyboard_set_textarea(kb, ta);
	} else if(e == LV_EVENT_DEFOCUSED) {
		int num,value;
		lv_obj_t * parent=lv_obj_get_parent(ta);
		lv_obj_t * child=lv_obj_get_child(parent, NULL);
		child=lv_obj_get_child(parent, child);

		char* temp= lv_label_get_text(child);
		sscanf(temp,"%d",&num);
		temp= lv_textarea_get_text(ta);
		sscanf(temp,"%d",&value);
		modbus_write_register(mb,num, value);
		lv_textarea_set_cursor_hidden(ta, true);
		lv_event_send(kb, LV_EVENT_CANCEL, NULL);


	}

}

void aoEnableSingleWrite(int status){
	for(int i=0;i<20;i++){
		if(irTa[i]!=NULL){
			if(status==1)
				lv_obj_set_event_cb(irTa[i], taEvent_Handler);
			else if(status==2)
				lv_obj_set_event_cb(irTa[i], ta_event_cb);
			else
				lv_obj_set_event_cb(irTa[i], NULL);
		}
	}

}

void initiateAo(int start,int count){
	char temp[10]={0};
	uint16_t tab_reg[20]={0};

	if(count>20){
		count=20;
		lv_textarea_set_text(aoQuantity, "20");
	}


	for(int i=0;i<20;i++){
		if(regCon[i]!=NULL){
			lv_obj_del(regCon[i]);
			regCon[i]=NULL;
			irTa[i]=NULL;
		}
	}
	if(mb!=NULL)
		modbus_read_registers(mb, start, count, tab_reg);



	lv_obj_t * parent=lv_obj_get_parent(aoCont);
	parent=lv_obj_get_parent(parent);
	lv_coord_t grid_w = lv_page_get_width_grid(parent,4, 1);




	for(int i=0;i<count;i++){
		regCon[i] = lv_cont_create(textAreaCont, NULL);
		lv_cont_set_layout(regCon[i], LV_LAYOUT_PRETTY_TOP);
		lv_obj_add_style(regCon[i], LV_CONT_PART_MAIN, &style_box);
		lv_cont_set_fit2(regCon[i], LV_FIT_NONE, LV_FIT_TIGHT);
		lv_obj_set_drag_parent(regCon[i], true);
		lv_obj_set_width(regCon[i], grid_w);
		irLbl[i] = lv_label_create(regCon[i], NULL);
		irTa[i] = lv_textarea_create(regCon[i], NULL);
		lv_textarea_set_text(irTa[i], "");
		lv_textarea_set_one_line(irTa[i], true);
		lv_textarea_set_cursor_hidden(irTa[i], true);
		lv_obj_set_width(irTa[i],3*grid_w/5 );
		//lv_obj_set_pos(irTa[i], 5, 20);

		/* Create a label and position it above the text box */
		lv_obj_align(irLbl[i], irTa[i], LV_ALIGN_OUT_LEFT_MID, 0, 0);
		sprintf(temp,"%02d",i+start);
		lv_label_set_text(irLbl[i], temp);
		sprintf(temp,"%d",tab_reg[i]);
		lv_textarea_set_text(irTa[i], temp);
	}

}
static lv_obj_t* createButton(char* text){
	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_obj_t* btn = lv_btn_create(aoCont, NULL);
	lv_btn_set_fit2(btn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(btn, lv_obj_get_width_grid(aoCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));

	lv_obj_set_event_cb(btn, aoRWBtnEvent_handler);
	lv_obj_t * okLabel = lv_label_create(btn, NULL);
	lv_label_set_text(okLabel ,text);
	return btn;
}

static void aoFuncEvent_handler(lv_obj_t * obj, lv_event_t event)
{


	if(event == LV_EVENT_VALUE_CHANGED) {
		char buf[32];
		lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
		if(aoRWBtn!=NULL){
			lv_obj_del(aoRWBtn);
			aoRWBtn=NULL;
		}
		if(aoUpdateBtn!=NULL){
			lv_obj_del(aoUpdateBtn);
			aoUpdateBtn=NULL;
		}

		if(strcmp(buf,"04: Read Input Register" )==0){

			aoRWBtn=createButton("Read");
			aoEnableSingleWrite(0);

		}
		if(strcmp(buf,"06: Write Single register" )==0)
		{
			aoUpdateBtn=createButton("Update");
			aoEnableSingleWrite(1);
		}
		if(strcmp(buf,"16: Write Multiple registers" )==0){
			aoUpdateBtn=createButton("Update");
			aoRWBtn=createButton("Write All");
			aoEnableSingleWrite(2);
		}

	}
}


void holdingRegister_create(lv_obj_t * parent){
	const char *temp;
	int address=0,quantity=0;
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

	lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);

	aoCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(aoCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(aoCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(aoCont, true);

	lv_cont_set_fit2(aoCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(aoCont, grid_w);
	lv_obj_set_style_local_value_str(aoCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Command");

	functionDd = lv_dropdown_create(aoCont, NULL);
	lv_dropdown_set_options(functionDd, "04: Read Input Register\n06: Write Single register\n16: Write Multiple registers");
	lv_obj_set_width(functionDd, grid_w-20);
	lv_obj_set_event_cb(functionDd, aoFuncEvent_handler);

	aoAddress = lv_textarea_create(aoCont, NULL);
	lv_textarea_set_placeholder_text(aoAddress, "Address");
	lv_textarea_set_text(aoAddress, "0");
	lv_textarea_set_one_line(aoAddress, true);
	lv_textarea_set_cursor_hidden(aoAddress, true);
	lv_obj_set_width(aoAddress, grid_w/2-20);
	lv_obj_set_event_cb(aoAddress, ta_event_cb);

	aoQuantity = lv_textarea_create(aoCont, NULL);
	lv_textarea_set_placeholder_text(aoQuantity, "Quantity");
	lv_textarea_set_text(aoQuantity, "20");
	lv_textarea_set_one_line(aoQuantity, true);
	lv_textarea_set_cursor_hidden(aoQuantity, true);
	lv_obj_set_width(aoQuantity, grid_w/2-20);
	lv_obj_set_event_cb(aoQuantity, ta_event_cb);

	aoRWBtn = lv_btn_create(aoCont, NULL);
	lv_btn_set_fit2(aoRWBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(aoRWBtn, lv_obj_get_width_grid(aoCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t * label = lv_label_create(aoRWBtn, NULL);
	lv_label_set_text(label ,"Read");
	lv_obj_set_event_cb(aoRWBtn, aoRWBtnEvent_handler);
	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent,  1, 1);


	textAreaCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(textAreaCont, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(textAreaCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(textAreaCont, true);

	lv_obj_set_style_local_value_str(textAreaCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Holding Register");

	lv_cont_set_fit2(textAreaCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(textAreaCont, grid_w);
	temp=lv_textarea_get_text(aoAddress);
	sscanf(temp,"%d",&address);
	temp=lv_textarea_get_text(aoQuantity);
	sscanf(temp,"%d",&quantity);
	initiateAo(address,quantity);
	aoEnableSingleWrite(0);
}
