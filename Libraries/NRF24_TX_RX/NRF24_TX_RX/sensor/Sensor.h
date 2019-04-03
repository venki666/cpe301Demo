/*
 * Sensor.h
 *
 *  Created on: Jun 2, 2015
 *      Author: mdanylov
 */

#ifndef SENSOR_H_
#define SENSOR_H_

/*
 * struct SENSOR represents a packet sent by sensor node.
 * Since more than one sensor is going to be used, we need
 * more sophisticated way of communication between bunch of sensors
 * and single receiver. it should be aware from which sensor
 * the data is received in order to handle that data properly furthermore
 */

#include <stdio.h>
#include <string.h>

typedef struct{
	uint8_t sensor_id;
	char* sensor_data;
}Sensor;

void sensor_set_id(Sensor* record, uint8_t id);
void sensor_set_data(Sensor* record, char* data);
uint8_t sensor_get_id(Sensor* record);
char* sensor_get_data(Sensor* record);
void sensor_to_str(Sensor* record, char* buff);

#endif /* SENSOR_H_ */
