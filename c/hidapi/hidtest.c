#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hidapi.h"


int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[66];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i, j;
	struct timespec req, rem;

	// Enumerate and print the HID devices on the system
	struct hid_device_info *devs, *cur_dev;
	
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
			cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);


	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x0416, 0xffff, NULL);

	// Read the Manufacturer String
        wstr[0] = '\0';
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
        wstr[0] = '\0';
	res = hid_get_product_string(handle, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
        wstr[0] = '\0';
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	printf("Serial Number String: %ls", wstr);
	printf("\n");
/*
	// Send a Feature Report to the device
	buf[0] = 0x2; // First byte is report number
	buf[1] = 0xa0;
	buf[2] = 0x0a;
	res = hid_send_feature_report(handle, buf, 17);

	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));

	// Print out the returned buffer.
        if (res > 0) {
		printf("Feature Report\n   ");
		for (i = 0; i < res; i++)
			printf("%02hhx ", buf[i]);
	} else {
		printf("No Feature Report");
	}
	printf("\n");
*/
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 0);


/*
	// Send an Output report to request the state (cmd 0x81)
	buf[1] = 0x81;
	hid_write(handle, buf, 65);
*/
	while(1) {
		if (!feof(stdin)) {
			buf[0] = 0; // First byte is report number
        		buf[1] = 1; // length
			buf[2] = (char)fgetc(stdin);
			res = hid_write(handle, buf, 5);
		}

	        memset(buf, 0, 65);
		res = hid_read(handle, buf, 65);
		if (res < 0)
			printf("Unable to read()\n");

                else if (res > 0) {
			// Print out the returned buffer.
			res = buf[0] >= sizeof(buf) ? sizeof(buf)-1 : buf[0];
                        buf[res+1] = '\0';
			printf("%s", buf+1);
		}
          
	}

	return 0;
}

