#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h>

int main ()
{
    wiringPiSetup();
  int fd ;

  if ((fd = serialOpen ("/dev/ttyS0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

// Loop, getting and printing characters

  for (;;)
  {
    serialPutchar(fd, 'b');
    
    putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
    sleep(2);
  }
}
