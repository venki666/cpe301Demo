/*
 * Sensor.c
 *
 *  Created on: Jun 2, 2015
 *      Author: mdanylov
 */

#include "Sensor.h"

void sensor_set_id(Sensor* sensor, uint8_t id){
	sensor->sensor_id = id;
}
void sensor_set_data(Sensor* sensor, char* data){
	sensor->sensor_data = data;
}
uint8_t sensor_get_id(Sensor* sensor){
	return sensor->sensor_id;
}
char* sensor_get_data(Sensor* sensor){
	return sensor->sensor_data;
}

void sensor_to_str(Sensor* record, char* buff){
	char* tmp_buff;//[16] = "!!!!!!!!!!!!!!!\0";
	sprintf(tmp_buff, "%d,", sensor_get_id(record));
	strcat(tmp_buff, sensor_get_data(record));
	strcat(tmp_buff, '\0');
	strcpy(buff, tmp_buff);
}

//void sensor_str_to_Sensor(Sensor* record, char* str){
//
//	char* tmp_buff;
//	sensor_set_id(record, atoi(strtok(str,",")));
//	sensor_set_id(record, strtok(str,","));
//}
