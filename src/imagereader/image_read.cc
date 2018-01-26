#include"image_read.h"
bool show = 1;



void find_coordinates(const char* input){
/*  int thresh = 155;
  int max_thresh = 255 ;
  Mat source, source_gray,source_gray_blurred,source_gray_blurred_thresh;
  source = imread(input,CV_LOAD_IMAGE_COLOR); //read the file

  namedWindow("DISPLAY window", WINDOW_NORMAL);
  resizeWindow("DISPLAY window",3000,3000);

  //temp is used whenever the function requires output can't directly overwrite input
  Mat source, temp;

  int thresh = 155; int max_thresh = 255 ;

  temp = imread(input,CV_LOAD_IMAGE_COLOR); //read the file
  cvtColor( temp, source, COLOR_BGR2GRAY );
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  GaussianBlur( source,source, Size(5,5), 0,0);
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  temp = source.clone();
  bilateralFilter(temp,source, 11,50,50);
  if(show){imshow("DISPLAY window",source );waitKey(0);}

  blur(source,source,Size(5,5) );
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  GaussianBlur( source, source, Size(3,3), 0, 0, BORDER_DEFAULT );
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  medianBlur( source, source, 11 );
  if(show){imshow("DISPLAY window", source);waitKey(0);}



  int scale = 3; int delta = 0; int ddepth = CV_16S;

  /// Generate Mat to store temporary gradient data
  Mat grad_x, grad_y,grad, abs_grad_x, abs_grad_y;

  //Apply eithe the Sobel or the Scharr algorithm to find edges
  Sobel( source, grad_x, ddepth, 1, 0, 1, scale, delta, BORDER_DEFAULT);
  Sobel( source, grad_y, ddepth, 0, 1, 1, scale, delta, BORDER_DEFAULT);

  //Scharr( source, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  //Scharr( source, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );

  convertScaleAbs( grad_x, abs_grad_x );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5,20, temp);
  if(show){imshow( "DISPLAY window", temp );waitKey(0);}

  source = temp.clone();
/*
  int threshold_value = 100; int max_BINARY_value = 200;
  threshold(source,source,threshold_value,max_BINARY_value,THRESH_BINARY);
  if(show){imshow("DISPLAY window", source);waitKey(0);}



  Ptr<CLAHE> clahe = createCLAHE();
  clahe->setClipLimit(1);
  clahe->apply(source,source);
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  equalizeHist( source,source );
  if(show){imshow("DISPLAY window", source);waitKey(0);}


  temp = source.clone();
  adaptiveThreshold(temp,source,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,5);
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  //GOOD FEATURES PARAMETERS
  vector<Point> corners;
  Mat mask; Mat drawing = source.clone();
  int maxCorners = 50;
  double qualityLevel = 0.01;
  double minDistance = 13.;
  int blockSize = 8;
  bool useHarrisDetector = true;
  double k = 0.04;

  goodFeaturesToTrack(source,corners,maxCorners,qualityLevel,minDistance,mask,blockSize,useHarrisDetector,k);
  for (size_t i=0; i<corners.size(); i++){
    //std::cout<<"x is " << dst[i].x<<std::endl;
    //std::cout<<"y is " << dst[i].y<<std::endl;
    circle(drawing, corners[i], 6, Scalar(40,32,203) , 2, 8, 0 );
  }
  std::cout << "size of corner vector is " << corners.size() << std::endl;

  std::cout << "flag 1" << std::endl;
  imshow( "DISPLAY window", drawing ); waitKey(0);

  vector<Vec4i> lines;
  HoughLinesP(source, lines, 1, CV_PI/180, 50, 0, 0 );
  std::cout<<"Size of hough line vector is " << lines.size()<< std::endl;
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( source, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
  imshow( "DISPLAY window", source ); waitKey(0);

  thresh = 155; max_thresh = 255; RNG rng(12345);

  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  temp = source.clone();
  /// Detect edges using Threshold
  threshold( source, threshold_output, thresh, 255, THRESH_BINARY );
  if(show){imshow("DISPLAY window", source);waitKey(0);}

  std::cout<<"flag 2" << std::endl;
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  std::cout<<"flag 3" << std::endl;
  std::cout << "size of contours vector is " << contours.size() << std::endl;

  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
  { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
  }
  std::cout << "size of boundRect vector is " << boundRect.size() << std::endl;

  std::cout<<"flag 4" << std::endl;

  /// Draw polygonal contour + bonding rects + circles
  Mat drawing1 = Mat::zeros( source.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
   {
     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
     drawContours( drawing1, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
     rectangle( drawing1, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
     circle( drawing1, center[i], (int)radius[i], color, 2, 8, 0 );
   }

    /// Show in a window
    imshow( "DISPLAY window", drawing1 );waitKey(0);


  //NEED TO FIND A WAY TO GROUP COORDINATES BY SHAPE


  bool rectHasPiece = false;
  int cornersContained =0;
  vector<Rectangle> usefulRectangles;
  vector<vector<int> > cornersOfPieces;
  //Filter out bounding rectangles to those that contain pieces
  for(int i=0;i<boundRect.size();i++){
    for(int j=0;j<corners.size();j++){
      if(boundRect[i].contains(corners[j])){rectHasPiece = true;cornersContained++;}
    }
    if(rectHasPiece){usefulRectangles.push_back(Rectangle(boundRect[i],boundRect[i].area()));}
    rectHasPiece = false;
  }

  std::cout << "size of usefulRect vector is " << usefulRectangles.size() << std::endl;
  std::cout << "corners contained is " << cornersContained << std::endl;
  std::cout << "Before sort is " << std::endl;

  for(int i=0;i<usefulRectangles.size();i++){
    std::cout << usefulRectangles[i].area << "    ";
  }
  std::cout << std::endl << "Now after sort is " << std::endl;

  std::sort(usefulRectangles.begin(),usefulRectangles.end(),compareByArea);

  for(int i=0;i<usefulRectangles.size();i++){
    std::cout << usefulRectangles[i].area << "    ";
  }

  std::cout << std::endl;

  usefulRectangles.erase(usefulRectangles.begin());

  std::cout << std::endl << "Now after delete is " << std::endl;

Mat drawing2 = Mat::zeros( threshold_output.size(), CV_8UC3 );
Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
  for(int i=0;i<usefulRectangles.size();i++){
    std::cout << usefulRectangles[i].area << "    ";
    rectangle( drawing2, usefulRectangles[i].rectangleOfAPolygon.tl(), usefulRectangles[i].rectangleOfAPolygon.br(), color, 2, 8, 0 );
  }

  namedWindow( "DISPLAY window", CV_WINDOW_AUTOSIZE );
  imshow( "DISPLAY window", drawing2 );
  waitKey(0);

  std::cout << std::endl;

  for(int i=0;i<usefulRectangles.size();i++){
    vector<int> coordinates;
    cornersOfPieces.push_back(coordinates);
    for(int j=0;j<corners.size();j++){
      if(usefulRectangles[i].rectangleOfAPolygon.contains(corners[j])){cornersOfPieces.at(i).push_back(corners[j].x);cornersOfPieces.at(i).push_back(corners[j].y);}
    }
  }
  std::cout << std::endl << "Size of cornersOfPieces is " << cornersOfPieces.size()<< std::endl;
  for(int i = 0;i<cornersOfPieces.size();i++){
    std::cout << i+1 <<"st shape has coordinates: ";
          std::cout<<"size is " << cornersOfPieces[i].size() << std::endl;
    for(int j =0; j<cornersOfPieces.at(i).size();j++){
      std::cout<<cornersOfPieces.at(i).at(j)<<"  ";
    }
    std::cout<<std::endl;
  }
*/
}


bool compareByArea(const Rectangle &a, const Rectangle &b){
  return a.area > b.area;
}
