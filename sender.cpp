#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
// #include "comm.h"

#define DATA_BUFFER_SIZE 1494
#define COMM_PORT "4950"
#define w 400
#define RECEIVER_IP "127.0.0.1"
// #define RECEIVER_IP "10.100.10.214"
#define SIZE_OF_MATRIX(X) ((X.total())*(X.elemSize()))

using namespace cv;

void MyEllipse( Mat img, double angle )
{
  int thickness = 2;
  int lineType = 8;

  ellipse( img,
       Point( w/2, w/2 ),
       Size( w/4, w/16 ),
       angle,
       0,
       360,
       Scalar( 255, 0, 0 ),
       thickness,
       lineType );
}

/**
 *  * @function MyFilledCircle
 *   * @brief Draw a fixed-size filled circle
 *    */
void MyFilledCircle( Mat img, Point center )
{
  int thickness = -1;
  int lineType = 8;

  circle( img,
      center,
      w/32,
      Scalar( 0, 0, 255 ),
      thickness,
      lineType );
}

int send_image(const char* const ip_address, Mat& img)
{

  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;

  unsigned char packt_data[1496];
  unsigned int ack;
  MatIterator_<Vec3b> it, end;
  size_t i=0;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(ip_address, COMM_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    return 2;
  }

  if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    perror("client: connect");
    close(sockfd);
  }

  for( it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; ++it)
  {
    packt_data[i++] = (*it)[0];
    packt_data[i++] = (*it)[1];
    packt_data[i++] = (*it)[2];

    if (i > (DATA_BUFFER_SIZE-3))
    {
      i=0;
      if ((numbytes = send(sockfd, packt_data, DATA_BUFFER_SIZE, 0)) == -1) {
        perror("talker: sendto");
        return(1);
      }
      // waitKey(1);
      if ((numbytes = recv(sockfd, (void*)&ack, 4 , 0)) == -1)
      {
          perror("talker: recv..");
          return(1);
      }
    }
  }

  if (i!=0)
  {
    if ((numbytes = send(sockfd, packt_data, DATA_BUFFER_SIZE, 0)) == -1) {
      perror("talker: sendto");
      return(1);
    }
  }

  freeaddrinfo(servinfo);
  close(sockfd);
}

int main ( void ){
	char atom_window[] = "Drawing: Atom, Sender";
	// Mat atom_image = Mat::zeros( w, w, CV_8UC3 );
  Mat atom_image = Mat(w, w, CV_8UC3, cv::Scalar(255,255,255));
  size_t i;
  // MatIterator_<Vec3b> it, end;

  unsigned char s[480000];

  //cv::Mat atom_image(100, 100, CV_8UC3);

  memset(s, 50, 1496);
  memset((s+1496), 51, 1496);
  memset((s+(2*1496)), 52, 1496);
  memset((s+(3*1496)), 53, 1496);
  memset((s+(4*1496)), 54, 1496);
  memset((s+(5*1496)), 55, 1496);
  memset((s+(6*1496)), 56, (10000 - (6*1496)));

  	MyEllipse( atom_image, 90 );
  	MyEllipse( atom_image, 0 );
  	MyEllipse( atom_image, 45 );
  	MyEllipse( atom_image, -45 );

	MyFilledCircle( atom_image, Point( w/2, w/2) );

  // for(it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
  // {
  //   for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
  //   {
  //       (*it)[0] = table[(*it)[0]];
  //       (*it)[1] = table[(*it)[1]];
  //       (*it)[2] = table[(*it)[2]];
  //   }
  // }

  // if (!atom_image.isContinuous()) {
  //     atom_image = atom_image.clone();
  // }

  // send_data(RECEIVER_IP,atom_image.data,SIZE_OF_MATRIX(atom_image));
  // s[9999] = 1;

  send_image(RECEIVER_IP,atom_image);

  // send_data(RECEIVER_IP,&s,sizeof s);

  // for(i=0;i<(sizeof s);i++)
  // {
  //   printf("s[%d]: %d\n",i,s[i]);
  // }

  // std::cout << SIZE_OF_MATRIX(atom_image) << std::endl;
  // printf("%s\n", );

	imshow( atom_window, atom_image );
  	// moveWindow( atom_window, 0, 200 );

	waitKey( 0 );
	return(0);
}
