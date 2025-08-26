#include "App.hpp"

// using namespace cv;

int main()
{
    // if ( argc != 2 )
    // {
    //     printf("usage: DisplayImage.out <Image_Path>\n");
    //     return -1;
    // }
    // Mat image;
    // image = imread( argv[1], 1 );
    // if ( !image.data )
    // {
    //     printf("No image data \n");
    //     return -1;
    // }
    // namedWindow("Display Image", WINDOW_AUTOSIZE );
    // imshow("Display Image", image);
    // waitKey(5000);
    // destroyWindow("Display Image");

   
    int returnCode = App::GetInstance().Init();
    if(returnCode == -1)
        return returnCode;
    returnCode = App::GetInstance().MainLoop();
    return returnCode;
}