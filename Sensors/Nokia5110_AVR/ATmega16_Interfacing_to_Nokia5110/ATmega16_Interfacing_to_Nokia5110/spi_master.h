/*
 * SPI_Master_H_file.h
 *
 * http://www.electronicwings.com
 */ 


#ifndef SPI_MASTER_H_FILE_H_
#define SPI_MASTER_H_FILE_H_

#include <avr/io.h>

#define MOSI 5
#define MISO 6
#define SCK 7
#define SS 4
#define DC 1
#define RST 0

void SPI_SS_Enable();
void SPI_SS_Disable();
void SPI_Init();
void SPI_Write(char write_data);
char SPI_Read();


#endif /* SPI_MASTER_H_FILE_H_ */