/*
 * connection.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */


/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "mainpage.h"
#include "stdio.h"


/**********************
 * VARIABLES
 **********************/

lv_obj_t * ipTa;
lv_obj_t * portTa;
extern lv_style_t style_box;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


//-------Connection Section
#include <main.h>

static void tcpBtnEvent_handler(lv_obj_t * obj, lv_event_t event)
{
	lv_obj_t * child=lv_obj_get_child(obj, NULL);;

    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
        tcpModbusInit();

        if(strcmp(lv_label_get_text(child),"Disconnected")==NULL){
        	if(tcpModbusConnect()==0){
        		lv_label_set_text(child,"Connected");
        		lv_btn_toggle(obj);
        	}
        }else{
        	lv_label_set_text(child,"Disconnected");
        	lv_btn_toggle(obj);
        	tcpModbusDisconnect();
        }
    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");

    }
}


void connection_create(lv_obj_t * parent)
{

	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

	lv_obj_t * rtuCont = lv_cont_create(parent, NULL);
	lv_cont_set_layout(rtuCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(rtuCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(rtuCont, true);

	lv_cont_set_fit2(rtuCont, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(rtuCont, grid_w);
	lv_obj_set_style_local_value_str(rtuCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "RTU");
	////------------------------------RTU
	lv_obj_t * serialPort = lv_dropdown_create(rtuCont, NULL);
	lv_dropdown_set_options(serialPort, "No port found");

	lv_obj_t * baudTa = lv_textarea_create(rtuCont, NULL);
	lv_cont_set_fit2(baudTa, LV_FIT_PARENT, LV_FIT_NONE);
	lv_textarea_set_placeholder_text(baudTa, "Baud Rate");
	lv_textarea_set_text(baudTa, "115200");
	lv_textarea_set_one_line(baudTa, true);
	lv_textarea_set_cursor_hidden(baudTa, true);
	lv_obj_set_event_cb(baudTa, ta_event_cb);


	lv_obj_set_width(serialPort, lv_obj_get_width(baudTa));


	lv_obj_t *  serialBit= lv_dropdown_create(rtuCont, NULL);
	lv_dropdown_set_options(serialBit, "8 Data bits\n7 Data bits");
	lv_obj_set_width(serialBit, lv_obj_get_width(baudTa));

	lv_obj_t *  serialParity= lv_dropdown_create(rtuCont, NULL);
	lv_dropdown_set_options(serialParity, "No Parity\nOdd Parity\nOdd Parity");
	lv_obj_set_width(serialParity, lv_obj_get_width(baudTa));

	lv_obj_t *  serialStopBit= lv_dropdown_create(rtuCont, NULL);
	lv_dropdown_set_options(serialStopBit, "1 Stop Bit\n2 Stop Bit");
	lv_obj_set_width(serialStopBit, lv_obj_get_width(baudTa));

	lv_obj_t * serialRefreshBtn = lv_btn_create(rtuCont, NULL);
	lv_btn_set_fit2(serialRefreshBtn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(serialRefreshBtn, lv_obj_get_width_grid(rtuCont, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t * label = lv_label_create(serialRefreshBtn, NULL);
	lv_label_set_text(label ,"Refresh");

	lv_obj_t *serialConnectBtn = lv_btn_create(rtuCont, serialRefreshBtn);

	label = lv_label_create(serialConnectBtn, NULL);
	lv_label_set_text(label ,"Disconnected");
	//--------------------------------------------------tcp/ip


	disp_size = lv_disp_get_size_category(NULL);
	grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);
	lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_MID);
	lv_obj_t * tcpCont = lv_cont_create(parent, rtuCont);
	lv_cont_set_layout(tcpCont, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(tcpCont, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(tcpCont, true);
	lv_cont_set_fit(tcpCont, LV_FIT_NONE);
	//lv_cont_set_fit2(tcpCont, LV_FIT_NONE, LV_FIT_TIGHT);
	//	lv_obj_set_width(tcpCont, grid_w);
	//	lv_obj_set_height(tcpCont, lv_obj_get_height(rtuCont));
	lv_obj_set_style_local_value_str(tcpCont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "TCP/IP");

	lv_obj_t *  tcpMasSla= lv_dropdown_create(tcpCont, NULL);
	lv_dropdown_set_options(tcpMasSla, "Master\nSlave");
	lv_obj_set_width(tcpMasSla, lv_obj_get_width(baudTa));

	lv_obj_t * slaveAddress = lv_textarea_create(tcpCont, NULL);
	lv_cont_set_fit2(slaveAddress, LV_FIT_PARENT, LV_FIT_NONE);
	lv_textarea_set_placeholder_text(slaveAddress, "Slave Address");
	lv_textarea_set_text(slaveAddress, "1");
	lv_textarea_set_one_line(slaveAddress, true);
	lv_textarea_set_cursor_hidden(slaveAddress, true);
	lv_obj_set_event_cb(slaveAddress, ta_event_cb);


	ipTa = lv_textarea_create(tcpCont, slaveAddress);

	lv_textarea_set_placeholder_text(ipTa, "Node IP");
	lv_textarea_set_text(ipTa, "127.0.0.1");

	portTa= lv_textarea_create(tcpCont, slaveAddress);
	lv_textarea_set_placeholder_text(portTa, "Port");
	lv_textarea_set_text(portTa, "502");
	lv_cont_set_fit4(portTa, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_fit2(portTa, LV_FIT_PARENT, LV_FIT_NONE);

	lv_obj_t *tcpMasterConnectBtn = lv_btn_create(tcpCont, serialRefreshBtn);

	label = lv_label_create(tcpMasterConnectBtn, NULL);
	lv_label_set_text(label ,"Disconnected");
	lv_obj_set_event_cb(tcpMasterConnectBtn, tcpBtnEvent_handler);


#if LV_DEMO_WIDGETS_SLIDESHOW
	tab_content_anim_create(parent);
#endif


}







