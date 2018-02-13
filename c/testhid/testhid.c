#include <stdio.h>
#include "libusb.h"

int main()
{
  libusb_context *pCTX = NULL;
  libusb_device **pDevList = NULL;
  libusb_device_handle *pDev;
  libusb_device_handle *hDev = NULL;
  struct libusb_config_descriptor *pConfig = NULL;
  struct libusb_device_descriptor Desc;
  int err;
  int NumDev, NumInterfaces;
  int i, j, k, l;

  err = libusb_init(&pCTX);
  if (err) {
    printf("Error: %d\n", err);
    pCTX = NULL;
  } else {
    printf("OK\n");
  }

  NumDev = libusb_get_device_list(pCTX, &pDevList);
  printf("Num devices: %d\n", NumDev);
  for(i = 0; i < NumDev; i++) {
    if (libusb_get_device_descriptor(pDevList[i], &Desc) != 0) {
      printf("Get desc failed for %d\n", i);
      continue;
    }
    printf("Device %d:\n", i);
    printf("  VID:PID 0x%x:0x%x\n", Desc.idVendor, Desc.idProduct);
    printf("  Speed: %d\n", libusb_get_device_speed(pDevList[i]));
    printf("  Num Config: %d\n", Desc.bNumConfigurations);
    if (Desc.idVendor == 0x0416 && Desc.idProduct == 0xffff) {
      if (Desc.bNumConfigurations > 0) {
        if (libusb_get_config_descriptor(pDevList[i], 0, &pConfig) == 0) {
          struct libusb_interface_descriptor id;

          NumInterfaces = pConfig->bNumInterfaces;
          for(j = 0; j < NumInterfaces; j++) {
            printf("  %d settings\n", pConfig->interface[j].num_altsetting);
            for(k = 0; k < pConfig->interface[j].num_altsetting; k++) {
              id = pConfig->interface[j].altsetting[k];
              printf("  if %d Class: %d\n", k, id.bInterfaceClass);
              printf("    %d endpoints\n", id.bNumEndpoints);
              for(l = 0; l < id.bNumEndpoints; l++) {
                printf("    Endpoint %d\n", l);
                printf("      Address: %d\n", id.endpoint[l].bEndpointAddress);
                printf("      Attributes 0x%x\n", id.endpoint[l].bmAttributes);
                printf("      MaxSize: %d\n", id.endpoint[l].wMaxPacketSize);
                printf("      Interval: %d\n", id.endpoint[l].bInterval);
              }
            }
          }
        }
      }

      if (libusb_open(pDevList[i], &hDev) == 0) {
        libusb_close(hDev);
      }

      if (pConfig) libusb_free_config_descriptor(pConfig);
      pConfig = NULL;

      if (Desc.bDeviceClass == LIBUSB_CLASS_HID) {
        printf("  Is HID\n");
      } else {
        printf("  Device Class %d\n", Desc.bDeviceClass);
      }
    }
  }
  if (pDevList) libusb_free_device_list(pDevList, 1);

/*  pDev = libusb_open_device_with_vid_pid(pCTX, 0x416, 0xFFFF);
  if (pDev) {
    printf("Found device!\n");
    libusb_close(pDev);
  } else {
    printf("Device not found.\n");
  }*/
  if (pCTX) {
    libusb_exit(pCTX);
  }
}
