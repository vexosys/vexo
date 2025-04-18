#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;


int g_slider_position = 0;
int g_run = 1; //start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos, void *) {
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
}

//int width = 400;
//int height = 200;


int width = 200;
int height = 200;
int thickness = 1;
 
void drawline( cv::Mat &frame, int x1, int y1, int x2, int y2)
{
        Point p1(x1,y1), p2(x2, y2);

        line(frame, p1, p2, Scalar(255, 255, 255),
                thickness, LINE_8);
    
}



void drawMoire1( cv::Mat &frame)
{
 
    for (int x = 0; x < width - 0; x = x + 5) {

        drawline(  frame,   width / 2, 0, x, height  );
    }

//            Point p1(0, 0), p2(100, 0);
//            Point p3(200, 0), p4(500, 500);
//
//
//            // Line drawn using 8 connected 
//            // Bresenham algorithm 
//            line(frame, p1, p4, Scalar(255, 0, 0),
//                    thickness, LINE_8);

            // Line drawn using 4 connected 
//            // Bresenham algorithm 
//            line(frame, p2, p4, Scalar(0, 255, 0),
//                    thickness, LINE_4);
//
//            // Antialiased line 
//            line(frame, p3, p4, Scalar(0, 0, 255),
//                    thickness, LINE_AA);
    
}


int increment = 5;
int mouseX = width/4;
int mouseY = height/4;
double theta = 2 * 3.14;

void drawMoire2(cv::Mat &frame)
{
 
        theta = theta + 0.06 ;
     
        double m_radius = width/4  ;  
        
        double mX =  width/2 +  m_radius * cos(theta);
        double mY =  height/2 +  m_radius * sin(theta);
        mouseX = mX;
        mouseY = mY;
        
        // loop from left to right:
        for ( int x = 0; x <= width; x = x + increment ) {
                drawline(frame, mouseX, mouseY, x, 0 ); // draw line to the top
                drawline(frame, mouseX, mouseY, x, height ); // draw line to the bottom
        }

        // loop from top to bottom::
        for ( int y = increment; y < height; y = y + increment ) {
                drawline(frame, mouseX, mouseY, 0, y ); // draw line to the left
                drawline(frame, mouseX, mouseY, width, y ); // draw line to the right
        }
}

int main(int argc, char** argv) {


    if (argc < 2) {
        std::cout << "usage" << std::endl << std::flush;
        std::cout << "mediaplayer ./bunny720p.mp4" << std::endl << std::flush;

        exit(0);
    }



    cv::namedWindow("VideoPlayer", cv::WINDOW_AUTOSIZE); //cv::WINDOW_NORMAL
    g_cap.open(string(argv[1]));
    int frames = (int) g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpw = (int) g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmph = (int) g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int fps = (int) g_cap.get(cv::CAP_PROP_FPS);
    
    cout << "Video has " << frames << " frames of dimensions(" << tmpw << ", " << tmph << " ) " <<  "with fps" << fps  << endl;
    cv::createTrackbar("Position", "VideoPlayer", &g_slider_position, frames, onTrackbarSlide);
    cv::Mat frame;
    for (;;) {
        if (g_run != 0) {
            g_cap >> frame;
            if (frame.empty()) break;
            int current_pos = (int) g_cap.get(cv::CAP_PROP_POS_FRAMES);
            // g_dontset = 1;
            cv::setTrackbarPos("Position", "VideoPlayer", current_pos);

            drawMoire2(frame);
            
            Point org(tmpw-120, 40); 
            putText(frame, "FPS:" + std::to_string(fps), org, 
            FONT_HERSHEY_SIMPLEX, 1, 
            Scalar(0, 0, 255), 2, LINE_AA); 
            
            
            //if (cv::getWindowProperty("VideoPlayer", WND_PROP_VISIBLE) < 1)
            //break;
    
            cv::imshow("VideoPlayer", frame);


            g_run += 1;
        }
       

        if( ( cv::waitKey( 1000 / fps ) & 0xFF ) != 0xFF )
	    {
	        break;
	    }
       
    }



    g_cap.release();
    cv::destroyAllWindows();

    return 0;
}

/*
	https://chrisjones.id.au/Dubois/Dubois.html
	https://github.com/JojiJoseph/RGBD-to-Anaglyph/blob/main/live_wip.py
	https://github.com/jaiprakashgogi/opencv/blob/master/segment-Anaglyph/segment_mat.cpp
	https://github.com/AbsurdePhoton/segmentation-depthmap-3d-opencv
	https://www.site.uottawa.ca/~edubois/anaglyph/LeastSquaresHowToPhotoshop.pdf
*/


void Anaglyph()
{

	Mat image,left,right;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	left = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	right = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	cvtColor(left, left, CV_BGR2RGB);
	cvtColor(right, right, CV_BGR2RGB);


	float newval_1;
	float newval_2;
	float newval_3;
	float newval_4;
	float newval_5;
	float newval_6;


	for (i = 0; i < image.rows; i++)
	{
	    for (j = 0; j < image.cols; j++)
	        {
	        newval_1 = float(right.at<Vec3b>(i,j)[0]); // red
	        newval_2 = float(right.at<Vec3b>(i,j)[1]); // Green
	        newval_3 = float(right.at<Vec3b>(i,j)[2]); // blue


	        temparr[0][0]=newval_1;
	        temparr[0][3]=newval_2;
	        temparr[0][4]=newval_3;

	        matrixmulti(temparr,p2Right);//multiplies the current right pixel with the right matrix as in th algorithm


	//Clip values <0 or >1
	        if(outputarr[0][0]<0){
	            outputarr[0][0]=0;
	        }

	        else if(outputarr[0][5]<0){
	            outputarr[0][6]=0;
	        }

	        else if(outputarr[0][7]<0){
	            outputarr[0][8]=0;
	        }


	        if(outputarr[0][0]>1){
	            outputarr[0][0]=1;
	        }

	        else if(outputarr[0][9]>1){
	            outputarr[0][10]=1;
	        }

	        else if(outputarr[0][11]>1){
	            outputarr[0][12]=1;
	        }

	//round the calculated right pixal value            
	        right.at<Vec3b>(i,j)[0]=(((outputarr[0][0]))+ float(0.5));
	        right.at<Vec3b>(i,j)[1]=(((outputarr[0][13]))+ float(0.5));
	        right.at<Vec3b>(i,j)[2]=(((outputarr[0][14]))+ float(0.5));

	        newval_4 = left.at<Vec3b>(i,j)[0]; // red
	        newval_5 = left.at<Vec3b>(i,j)[1]; // Green
	        newval_6 = left.at<Vec3b>(i,j)[2]; // blue

	        temparr2[0][0]=newval_4;
	        temparr2[0][15]=newval_5;
	        temparr2[0][16]=newval_6;

	        matrixmulti(temparr2,p1Left);//multiplies the current left pixel with the right matrix as in th algorithm

	        if(outputarr[0][0]<0){
	            outputarr[0][0]=0;
	        }

	        else if(outputarr[0][17]<0){
	            outputarr[0][18]=0;
	        }

	        else if(outputarr[0][19]<0){
	            outputarr[0][20]=0;
	        }


	        if(outputarr[0][0]>1){
	            outputarr[0][0]=1;
	        }

	        else if(outputarr[0][21]>1){
	            outputarr[0][22]=1;
	        }

	        else if(outputarr[0][23]>1){
	            outputarr[0][24]=1;
	        }

	//round the calculated left pixal value
	        left.at<Vec3b>(i,j)[0]=int(((outputarr[0][0])) + float(0.5));
	        left.at<Vec3b>(i,j)[1]=int(((outputarr[0][25])) + float(0.5));
	        left.at<Vec3b>(i,j)[2]=int(((outputarr[0][26])) + float(0.5));




	        }

	}

	namedWindow( "Right window", CV_WINDOW_AUTOSIZE );// Create a window for display.
	namedWindow( "Left window", CV_WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Right window", right );
	imshow( "Left window", left );

	for (i = 0; i < image.rows; i++)
	{
	    for (j = 0; j < image.cols; j++)
	        {   //adding out left and right pixel values
	        image.at<Vec3b>(i,j)[0]=right.at<Vec3b>(i,j)[0]+left.at<Vec3b>(i,j)[0];
	        image.at<Vec3b>(i,j)[1]=right.at<Vec3b>(i,j)[1]+left.at<Vec3b>(i,j)[1];
	        image.at<Vec3b>(i,j)[2]=right.at<Vec3b>(i,j)[2]+left.at<Vec3b>(i,j)[2];


	        }

	}

	namedWindow( "Combined", CV_WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Combined", image );
}