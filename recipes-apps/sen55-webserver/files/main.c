/*
 * I2C-Generator: 0.3.0
 * Yaml Version: 2.1.3
 * Template Version: 0.7.0-109-gb259776
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*Based on sen5x_i2c_example_usage.c of Sensirion*/
/*Modified by jersonvm and saved as main.c*/

#include <stdio.h>   // printf
#include <string.h>  // strcmp

#include "sen5x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"



int main(void) {
    int16_t error = 0;

    sensirion_i2c_hal_init();

    error = sen5x_device_reset();
    if (error) {
        printf("Error executing sen5x_device_reset(): %i\n", error);
    }

    unsigned char serial_number[32];
    uint8_t serial_number_size = 32;
    error = sen5x_get_serial_number(serial_number, serial_number_size);
    if (error) {
        printf("Error executing sen5x_get_serial_number(): %i\n", error);
    } else {
        printf("Serial number: %s\n", serial_number);
    }

    unsigned char product_name[32];
    uint8_t product_name_size = 32;
    error = sen5x_get_product_name(product_name, product_name_size);
    if (error) {
        printf("Error executing sen5x_get_product_name(): %i\n", error);
    } else {
        printf("Product name: %s\n", product_name);
    }

    uint8_t firmware_major;
    uint8_t firmware_minor;
    bool firmware_debug;
    uint8_t hardware_major;
    uint8_t hardware_minor;
    uint8_t protocol_major;
    uint8_t protocol_minor;
    error = sen5x_get_version(&firmware_major, &firmware_minor, &firmware_debug,
                              &hardware_major, &hardware_minor, &protocol_major,
                              &protocol_minor);

    if (error) {
        printf("Error executing sen5x_get_version(): %i\n", error);
    } else {
        printf("Firmware: %u.%u, Hardware: %u.%u\n", firmware_major,
               firmware_minor, hardware_major, hardware_minor);
    }

    // set a temperature offset - supported by SEN54 and SEN55 sensors
    //
    // By default, the temperature and humidity outputs from the sensor
    // are compensated for the modules self-heating. If the module is
    // designed into a device, the temperature compensation might need
    // to be adapted to incorporate the change in thermal coupling and
    // self-heating of other device components.
    //
    // A guide to achieve optimal performance, including references
    // to mechanical design-in examples can be found in the app note
    // “SEN5x – Temperature Compensation Instruction” at www.sensirion.com.
    // Please refer to those application notes for further information
    // on the advanced compensation settings used in
    // `sen5x_set_temperature_offset_parameters`,
    // `sen5x_set_warm_start_parameter` and
    // `sen5x_set_rht_acceleration_mode`.
    //
    // Adjust temp_offset in degrees celsius to account for additional
    // temperature offsets exceeding the SEN module's self heating.
    float temp_offset = 0.0f;
    int16_t default_slope = 0;
    uint16_t default_time_constant = 0;
    error = sen5x_set_temperature_offset_parameters(
        (int16_t)(200 * temp_offset), default_slope, default_time_constant);
    if (error) {
        printf(
            "Error executing sen5x_set_temperature_offset_parameters(): %i\n",
            error);
    } else {
        printf("Temperature Offset set to %.2f °C (SEN54/SEN55 only)\n",
               temp_offset);
    }

    // Start Measurement
    error = sen5x_start_measurement();
    if (error) {
        printf("Error executing sen5x_start_measurement(): %i\n", error);
    }



    while(1) {
        // Read Measurement
        sensirion_i2c_hal_sleep_usec(5000000);

        uint16_t mass_concentration_pm1p0;
        uint16_t mass_concentration_pm2p5;
        uint16_t mass_concentration_pm4p0;
        uint16_t mass_concentration_pm10p0;
        int16_t ambient_humidity;
        int16_t ambient_temperature;
        int16_t voc_index;
        int16_t nox_index;

        error = sen5x_read_measured_values(
            &mass_concentration_pm1p0, &mass_concentration_pm2p5,
            &mass_concentration_pm4p0, &mass_concentration_pm10p0,
            &ambient_humidity, &ambient_temperature, &voc_index, &nox_index);

        if (error) {
            printf("Error executing sen5x_read_measured_values(): %i\n", error);
        } else {
            printf("Mass concentration pm1p0: %.1f µg/m³\n",
                   mass_concentration_pm1p0 / 10.0f);
            printf("Mass concentration pm2p5: %.1f µg/m³\n",
                   mass_concentration_pm2p5 / 10.0f);
            printf("Mass concentration pm4p0: %.1f µg/m³\n",
                   mass_concentration_pm4p0 / 10.0f);
            printf("Mass concentration pm10p0: %.1f µg/m³\n",
                   mass_concentration_pm10p0 / 10.0f);
            if (ambient_humidity == 0x7fff) {
                printf("Ambient humidity: n/a\n");
            } else {
                printf("Ambient humidity: %.1f %%RH\n",
                       ambient_humidity / 100.0f);
            }
            if (ambient_temperature == 0x7fff) {
                printf("Ambient temperature: n/a\n");
            } else {
                printf("Ambient temperature: %.1f °C\n",
                       ambient_temperature / 200.0f);
            }
            if (voc_index == 0x7fff) {
                printf("Voc index: n/a\n");
            } else {
                printf("Voc index: %.1f\n", voc_index / 10.0f);
            }
            if (nox_index == 0x7fff) {
                printf("Nox index: n/a\n");
            } else {
                printf("Nox index: %.1f\n", nox_index / 10.0f);
            }
        }

        FILE *fp;
        /*fp = fopen("sen55.txt","w");*/
        fp = fopen("index.html","w");

        if(!fp) {
        	perror("fopen");
        	return 1;
        }

        /*if(fprintf(fp,"%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f",
        		mass_concentration_pm1p0/ 10.0f,
				mass_concentration_pm2p5/ 10.0f,
				mass_concentration_pm4p0/ 10.0f,
				mass_concentration_pm10p0/ 10.0f,
				ambient_humidity / 100.0f,
				ambient_temperature / 200.0f,
				voc_index / 10.0f,
				nox_index / 10.0f
			) == EOF) {
        	perror("fprintf");
        	return 1;
        }*/

        if(fprintf(fp, "\
        		<!DOCTYPE html>\n \
				<html>\n \
				<head>\n \
				<title> SEN55 Web Server on Avnet MiniZed </title>\n \
				<link type=\"text/css\" rel=\"stylesheet\" href=\"css/main.css\" />\n \
				<meta http-equiv=\"refresh\" content=\"5\">\n \
				<script>\n \
				</script>\n \
				</head>\n \
				<body>\n \
				<div id=\"container\">\n \
				<h1>SEN55 Web Server on Avnet MiniZed</h1>\n \
				<p> This is a demonstration of a simple sensor embedded webserver using the Avnet MiniZed board and SEN55 all-in-one environmental sensor\n \
				</p>\n \
				<h2> Sensor Reading </h2>\n \
				<p>\n \
				Mass Concentration PM1.0 = %.1f <br>\n \
				Mass Concentration PM2.5 = %.1f <br>\n \
     			Mass Concentration PM4.0  = %.1f <br>\n \
				Mass Concentration PM10.0  = %.1f <br>\n \
				Ambient Humidity = %.1f <br>\n \
				Ambient Temperature = %.1f <br>\n \
				VOC Index = %.1f <br>\n \
				NOx Index = %.1f <br>\n \
				</p>\n \
				</div>\n \
				</body>\n \
				</html>",
                mass_concentration_pm1p0/ 10.0f,
        		mass_concentration_pm2p5/ 10.0f,
        		mass_concentration_pm4p0/ 10.0f,
        		mass_concentration_pm10p0/ 10.0f,
        		ambient_humidity / 100.0f,
        		ambient_temperature / 200.0f,
        		voc_index / 10.0f,
        		nox_index / 10.0f
        		) == EOF) {
                	perror("fprintf");
                	return 1;
        }

        if(fclose(fp) == EOF) {
        	perror("fprintf");
        	return 1;
        }
    }

    error = sen5x_stop_measurement();
    if (error) {
        printf("Error executing sen5x_stop_measurement(): %i\n", error);
    }

    return 0;
}
