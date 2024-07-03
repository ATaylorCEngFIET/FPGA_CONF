#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>


#define UIO_SIZE "/sys/class/uio/uio4/maps/map0/size"
#define UIO_ADDR "/sys/class/uio/uio4/maps/map0/addr"

static int GPIO_UIO;

int main()
{
	unsigned int uio_addr;
	unsigned int uio_size;
	FILE * size_gpio;
	FILE * size_addr;
	void *ptr;
	uint32_t enable = 1;
	ssize_t en;
	unsigned int led_value;

    GPIO_UIO = open("/dev/uio4", O_RDWR);
    if (GPIO_UIO < 0) {
      perror("UIO Open Error:");

    }

    size_gpio = fopen(UIO_SIZE, "r");
    if (size_gpio == NULL){
    	perror("UIO Size Open Error:");
    }
    fscanf(size_gpio,"0x%16X",&uio_size);
    printf("size of UIO Memory: 0x%x\n",uio_size);

    size_addr = fopen(UIO_ADDR, "r");
    if (size_addr == NULL){
    	perror("UIO Addr Open Error:");
    }
    fscanf(size_addr,"0x%16X",&uio_addr);
    printf("size of UIO Memory: 0x%x\n",uio_addr);



    ptr = mmap(NULL, uio_size, PROT_READ|PROT_WRITE, MAP_SHARED, GPIO_UIO, 0);
    if (ptr == MAP_FAILED) {
       perror("mmap error:");
     }

    for(int i = 0; i<100; i++){

    	*(volatile unsigned int *) (ptr + 0x00) = 0x04;
    	usleep(500000);
    	*(volatile unsigned int *) (ptr + 0x00) = 0x00;
    	usleep(500000);
    }



    return 0;
}
