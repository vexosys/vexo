#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;


int g_slider_position = 0;
int64 g_run = 1; //start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos, void *) {
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
}




int width = 200;
int height = 200;
int thickness = 10;

    
const double PI = 3.14159265358979323846;
double radius = 50.0; // Radius of the circle
double centerX = width/2;; // X-coordinate of the center
double centerY = height/2;; // Y-coordinate of the center

double angle = 0;

 
void drawline( cv::Mat &frame, int x1, int y1, int x2, int y2)
{
    Point p1(x1,y1), p2(x2, y2);

    line(frame, p1, p2, Scalar(255, 255, 255),
                thickness, LINE_8);
    
}



void drawMoireFan( cv::Mat &frame)
{

    if( angle >  2 * PI)
    angle = 0;
            
            
    angle += 0.1;  // 5.72958 degree
    {
        
        // double x = centerX + radius * cos(angle);
        // double y = centerY + radius * sin(angle);
        // drawline(  frame,   centerX , centerY , x, y  );


        {
            double x1 = centerX + radius * cos(angle + PI/2);
            double y1 = centerY + radius * sin(angle + PI/2);

            double x2 = centerX + radius * cos(angle + PI + PI/2 );
            double y2 = centerY + radius * sin(angle + PI + PI/2 );

            drawline(  frame,   x1, y1 , x2, y2  );
        }


       {
            double x1 = centerX + radius * cos(angle);
            double y1 = centerY + radius * sin(angle);

            double x2 = centerX + radius * cos(angle + PI );
            double y2 = centerY + radius * sin(angle + PI);

            drawline(  frame,   x1, y1 , x2, y2  );
        }


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
    cv::Mat fmclone;
  
    int64 t0 = cv::getTickCount();
    
    int64 nfrm = 0;
    
    uint64_t frame_delay = (1.0f/fps) * 1000ull * 1000ull * 1000ull;

    for (;;) {
    	

        if (g_run != 0) {
            

           
            int64 t1 = cv::getTickCount();
          
            int64 ttmp =  t1-t0 ;
             
            int64 nanosecs = (t1-t0)*(cv::getTickFrequency()/1000000000);

            
            if( nanosecs  >  nfrm*frame_delay)
            {
                g_cap >> frame; 
                nfrm = nfrm +1;
                
               // std::cout << " nanosecs "  << nanosecs  << " nfrm*frame_delay " << nfrm*frame_delay << std::endl;
                
               // std::cout << " screen refress rate "  << 1000000000*g_run/ nanosecs <<  " rpm " <<  (1000000000*g_run)/ (nanosecs*62.8) << std::endl;
                
            }
            
                
            fmclone = frame.clone();
                    	
            
            if (frame.empty()) break;
            int current_pos = (int) g_cap.get(cv::CAP_PROP_POS_FRAMES);
            // g_dontset = 1;
            //cv::setTrackbarPos("Position", "VideoPlayer", current_pos);

            drawMoireFan(fmclone);
            
            // Point org(tmpw-120, 40); 
            // putText(frame, "FPS:" + std::to_string(fps), org, 
            // FONT_HERSHEY_SIMPLEX, 1, 
            // Scalar(0, 0, 255), 2, LINE_AA); 
            
            
            //if (cv::getWindowProperty("VideoPlayer", WND_PROP_VISIBLE) < 1)
            //break;
    
            cv::imshow("VideoPlayer", fmclone);

            
            g_run += 1;
        }
       
       
        if( ( cv::waitKey( 1 ) & 0xFF ) != 0xFF )
	    {
	        break;
	    }


       
    }



    g_cap.release();
    cv::destroyAllWindows();

    return 0;
}
