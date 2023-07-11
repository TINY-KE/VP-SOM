#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <sys/select.h>

#include <time.h>

#include "common_include.h"


//设置波特率函数
int set_speed(int fd, int speed)
{
	int speed_arr[]={B115200, B9600, B57600};
	int name_arr[]={115200, 9600, 57600};
	int i;
	int stat;
	struct termios opt;
	tcgetattr(fd, &opt);

	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt, speed_arr[i]);
			cfsetospeed(&opt, speed_arr[i]);
			stat=tcsetattr(fd, TCSANOW, &opt);

			if (stat != 0) {
				perror("tcsetattr fd");
				exit(-1);
			}

			tcflush(fd, TCIOFLUSH);
			return 0;
		}
	}

	return -1;
}
//设置校验位，数据位，停止位
int set_parity(int fd, int databits, int stopbits, char parity)
{
	struct termios opt;

	if (tcgetattr(fd, &opt) != 0) {
		perror("set up serial ");
		return-1;
	}

	opt.c_cflag &= ~CSIZE;

	switch (databits) {
		case 7:
			opt.c_cflag |= CS7;
			break;
		case 8:
			opt.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr, "unsupported data bits");
			return -1;
			break;
	}

	switch (parity) {
		case 'n':
		case 'N':
			opt.c_cflag &= ~PARENB;
			opt.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			opt.c_cflag |= (PARENB|PARODD);
			break;
		case 'e':
		case 'E':
			opt.c_cflag |= PARENB;
			opt.c_cflag &= ~PARODD;
			break;
		case 's':
		case 'S':
			opt.c_cflag &= ~PARENB;
			opt.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr, "unsuppoerted parity\n");
			return -1;
	}

	switch (stopbits) {
		case 1:
			opt.c_cflag &= ~CSTOPB;
			break;
		case 2:
			opt.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr, "unsuppoerted stop bits\n");
			return -1;
	}

	if ((parity != 'n') && (parity != 'N')) {
		opt.c_iflag |= INPCK;
	}

	opt.c_iflag &= ~(ICRNL | IXON);
	opt.c_cflag |= (CLOCAL | CREAD);
	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	opt.c_oflag &= ~OPOST;

	/*opt.c_cflag &=~CRTSCTS;
	opt.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL);
	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	opt.c_oflag &= ~(ONLCR | OCRNL | OPOST);
	
	opt.c_cflag |= (CLOCAL | CREAD);*/
	tcflush(fd, TCIFLUSH);

	opt.c_cc[VTIME]=0;
	

	opt.c_cc[VMIN]=0;

	if (tcsetattr(fd, TCSANOW, &opt) != 0) {
		perror("set up serial\n");
		return -1;
	}
	return 0;

}
//打开串口
int open_serial(char *dev, int speed, int data_bits, char patity, int stopbits)
{
	int fd, rc;

	fd = open(dev, O_RDWR|O_NOCTTY);
	if (fd < 0) {
		perror("cann't open serial port");
		return -1;
	}

	rc = set_parity(fd, data_bits, stopbits, patity);
	if (rc < 0)
		return -1;

	rc = set_speed(fd, speed);
	if (rc < 0)
		return -1;

	return fd;
}

int openDev(char *dev)
{
	int fd = open(dev, O_RDWR|O_NOCTTY);

	if (-1 == fd) {
		perror("cann't open serial port ");
		return -1;
	} else 
		return fd;
}


// int main()
// {
// 	int ret;
// 	char buf[128];
//     pthread_t th;
// 	int fd = open_serial(PORT, BAUDRATE, 8, 'N', 1);
//     if (fd < 0)
//     {
//         perror("cann't open serial port ");
//         return -1;
//     }
// 	//创建接收线程，用于读取串口数据
//     pthread_create(&th, NULL, pthread_read, &fd);
// 	while (1)
// 	{
// 		//获取终端数据，打印到串口
// 		if (NULL != fgets(buf, sizeof buf, stdin))
// 		{
// 			write(fd, buf, strlen(buf));
// 		}
// 	}	
// }
