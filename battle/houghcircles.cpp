#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
            "Usage:\n"
            "./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}

int main(int argc, char** argv)
{	
	CvCapture* capture = 0;
	Mat frame, frameCopy;
	string inputName;
	const char* filename = argc >= 2 ? argv[1] : "../data/board.jpg";
	cvNamedWindow( "result", 1 );
	{
		capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
		int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
        	if(!capture) cout << "Capture from CAM " <<  c << " didn't work" << endl;
	}

	if( capture ){
		cout << "In capture ..." << endl;
		for(;;){
			IplImage* iplImg = cvQueryFrame( capture );
			frame = cv::cvarrToMat(iplImg);
			if( frame.empty() )
				break;
			if( iplImg->origin == IPL_ORIGIN_TL )
				frame.copyTo( frameCopy );
			else
				flip( frame, frameCopy, 0 );
			
			Mat gimg;
			medianBlur(frameCopy, frameCopy, 5);
			cvtColor(frameCopy, gimg, COLOR_RGB2GRAY);
			vector<Vec3f> circles;
			HoughCircles(gimg, circles, HOUGH_GRADIENT, 1, 10,
				100, 30, 1, 30	// change the last two parameters
						// (min_radius & max_radius) to detect larger circles
				);
			for( size_t i = 0; i < circles.size(); i++ ){
				Vec3i c = circles[i];
				circle( frameCopy, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
				circle( frameCopy, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
			}
    			cv::imshow( "result", frameCopy );
			if( waitKey( 10 ) >= 0 )
				goto _cleanup_;
		}
_cleanup_:
        cvReleaseCapture( &capture );
    }

    return 0;
}
