//
//  ViewController.m
//  FaceMotionTracking
//
//  Created by Alvin on 11/30/15.
//  Copyright © 2015 Alvin. All rights reserved.
//

#import "ViewController.h"
#import <GPUImage/GPUImage.h>
#ifdef __cplusplus
#include <opencv2/opencv.hpp> // Includes the opencv library
#include <stdlib.h> // Include the standard library
#endif


@interface ViewController ()
@property (nonatomic, strong) CIDetector *faceDetector;
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *previewLayer;


@end

@implementation ViewController

@synthesize imagePreview, videoOutput, leftEyeTest, rightEyeTest, unlockedView;

int _cameraWidth  = 480;
int _cameraHeight = 640;

CGPoint leftCentroidCVFrame = CGPointMake(0, 0);
CGPoint rightCentroidCVFrame = CGPointMake(0, 0);

int state = 0;
CGFloat screenWidth = 768;
CGFloat screenHeight = 1024;
CGFloat offset = 0;


NSString* _defaultAVCaptureSessionPreset = AVCaptureSessionPreset640x480;

NSString *dotFilePath2 = [[NSBundle mainBundle] pathForResource:@"images/dot2" ofType:@"png"];
UIImage *dotImage2 = [UIImage imageWithContentsOfFile:dotFilePath2];
UIImageView *dotArrow2 = [[UIImageView alloc] initWithImage:dotImage2];

NSString *leftFilePath2 = [[NSBundle mainBundle] pathForResource:@"images/leftArrow2" ofType:@"png"];
UIImage *leftArrowImage2 = [UIImage imageWithContentsOfFile:leftFilePath2];
UIImageView *leftArrow2 = [[UIImageView alloc] initWithImage:leftArrowImage2];

NSString *rightFilePath2 = [[NSBundle mainBundle] pathForResource:@"images/rightArrow2" ofType:@"png"];
UIImage *rightArrowImage2 = [UIImage imageWithContentsOfFile:rightFilePath2];
UIImageView *rightArrow2 = [[UIImageView alloc] initWithImage:rightArrowImage2];

NSString *unlockedPath = [[NSBundle mainBundle] pathForResource:@"images/unlockedScreen" ofType:@"png"];
UIImage *unlockedImage = [UIImage imageWithContentsOfFile:unlockedPath];
UIImageView *unlockedImageView = [[UIImageView alloc] initWithImage:unlockedImage];


AVCaptureSession *session;
AVCaptureVideoOrientation _defaultAVCaptureVideoOrientation = AVCaptureVideoOrientationPortrait; 	//portrait/landscape
dispatch_queue_t videoQueue;

std::vector<CGPoint> leftEyeAverage;
std::vector<CGPoint> rightEyeAverage;
CGPoint leftAvg;
CGPoint rightAvg;

bool isCalibrated = false;
bool backToCenter = true;
int frameCount = 0;
CGPoint leftCalibrated;
CGPoint rightCalibrated;

std::string inputPattern;
std::string lockPattern = "0LR";

// should the camera image be like a mirror?

bool _mirrored = true;

- (void)viewDidLoad {
    [super viewDidLoad];

    [self initializeCamera];
    NSDictionary *detectorOptions = [[NSDictionary alloc] initWithObjectsAndKeys:CIDetectorAccuracyLow, CIDetectorAccuracy, @(YES), CIDetectorTracking, nil];
    self.faceDetector = [CIDetector detectorOfType:CIDetectorTypeFace context:nil options:detectorOptions];
    
    [self initializePattern];
    
    
    UITapGestureRecognizer *singleFingerTap =
    [[UITapGestureRecognizer alloc] initWithTarget:self
                                            action:@selector(handleSingleTap:)];
    [self.view addGestureRecognizer:singleFingerTap];
}

- (void)handleSingleTap:(UITapGestureRecognizer *)recognizer {
    if(state == 0){
        screenWidth = 240;
        screenHeight = 320;
        offset = 1024-320;
        state = 1;
        [self.leftEyeTest setHidden:YES];
        [self.rightEyeTest setHidden:YES];
    }else{
        screenWidth = 768;
        screenHeight = 1024;
        offset = 0;
        state = 0;
        [self.leftEyeTest setHidden:NO];
        [self.rightEyeTest setHidden:NO];
        [self.unlockedView setHidden:YES];
        for(int i = 0; i < inputPattern.size(); i++){
            if(inputPattern.at(i) == '0'){
                UIView *viewCenterRemove = [self.centerDotView viewWithTag:3];
                [viewCenterRemove removeFromSuperview];
            }else if(inputPattern.at(i) == 'L'){
                UIView *viewLeftRemove = [self.leftArrowView viewWithTag:1];
                [viewLeftRemove removeFromSuperview];
            }else if(inputPattern.at(i) == 'R'){
                UIView *viewRightRemove = [self.rightArrowView viewWithTag:2];
                [viewRightRemove removeFromSuperview];
            }
        }
    }
    CGRect newSize = self.imagePreview.frame;
    newSize.size.width = screenWidth;
    newSize.size.height = screenHeight;
    newSize.origin.y = offset;
    [self.imagePreview setFrame:newSize];
    self.previewLayer.frame = self.imagePreview.frame;
}

// Delegate routine that is called when a sample buffer was written
- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection
{
    
    // Create autorelease pool because we are not in the main_queue
    @autoreleasepool {
        
        // Get a CMSampleBuffer's Core Video image buffer for the media data
        CVPixelBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
        
        // Lock the base address of the pixel buffer
        CVPixelBufferLockBaseAddress(imageBuffer, 0);
        
        // Get the pixel buffer width and height
        int width = (int)CVPixelBufferGetWidth(imageBuffer);
        int height = (int)CVPixelBufferGetHeight(imageBuffer);
        
        if(width != _cameraWidth || height != _cameraHeight) {
            
            [connection setVideoOrientation: 	_defaultAVCaptureVideoOrientation];
            [connection setVideoMirrored: 		_mirrored];
            
            //We unlock the  image buffer
            CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
            
        } else {
            
            CFDictionaryRef attachments = CMCopyDictionaryOfAttachments(kCFAllocatorDefault, sampleBuffer, kCMAttachmentMode_ShouldPropagate);
            CIImage *ciImage = [[CIImage alloc] initWithCVPixelBuffer:imageBuffer
                                                              options:(__bridge NSDictionary *)attachments];
            if (attachments) {
                CFRelease(attachments);
            }
            
//            UIImage *tmpImage = [UIImage imageNamed:@"right.jpg"];
////            UIImage *tmpImage = [UIImage imageNamed:@"test.jpg"];
//            UIGraphicsBeginImageContext(self.imagePreview.bounds.size);
//            [tmpImage drawInRect:CGRectMake(0.0, 0.0, self.imagePreview.bounds.size.width, self.imagePreview.bounds.size.height)];
//            UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
//            UIGraphicsEndImageContext();
//            CIImage* ciImage = [[CIImage alloc] initWithImage:imageCopy];
//            //NSLog(@"size %i", (int)imageCopy.size.height);
            
            NSDictionary *imageOptions = nil;
            NSNumber *orient = [[NSNumber alloc] initWithInt:1];
            imageOptions = [NSDictionary dictionaryWithObject:orient
                                                       forKey:CIDetectorImageOrientation];
            NSArray *features = [self.faceDetector featuresInImage:ciImage
                                                           options:imageOptions];
            
            dispatch_async(dispatch_get_main_queue(), ^(void) {
                [self drawFaces:features
                    sourceImage:ciImage];
            });
        }
    }
}

-(CGPoint)getCentroid:(CGRect&)eyeRect
        originalImage:(CIImage*) ciImage
 setCentroidInCVFrame:(CGPoint&) cvFrame
          leftOrRight:(int) side
{
    CIContext *tempContext = [CIContext contextWithOptions:nil];
    CGImageRef imageRef = [tempContext createCGImage:ciImage fromRect:eyeRect];
    //    CGImageRef imageRef = CGImageCreateWithImageInRect(ciImage, eyeRect);
    UIImage* eyePatch = [UIImage imageWithCGImage:imageRef];
//    
//    GPUImagePicture *stillImageSource = [[GPUImagePicture alloc] initWithImage:eyePatch];
//    GPUImageColorInvertFilter *invertImageFilter = [[GPUImageColorInvertFilter alloc] init];
//    GPUImageGrayscaleFilter *grayImageFilter = [[GPUImageGrayscaleFilter alloc] init];
//    GPUImageLuminanceThresholdFilter *thresholdImageFilter =[[GPUImageLuminanceThresholdFilter alloc] init];
//    thresholdImageFilter.threshold = 0.8;
//    [stillImageSource addTarget:invertImageFilter];
//    [invertImageFilter addTarget:grayImageFilter];
//    [grayImageFilter addTarget:thresholdImageFilter];
//    // Process the image
//    [stillImageSource processImage];
//    UIImage* cv_img = [grayImageFilter imageFromCurrentFramebuffer];

    cv::Mat cv_img = [self cvMatFromUIImage:eyePatch];
    cv::Mat cv_gray;
    
    cv::cvtColor(~cv_img, cv_gray, CV_BGR2GRAY );
    cv::vector<cv::Vec3f> centroids;
    int thresh = 220;
    //int ksize =5;
    //cv::Mat cv_gauss; cv::GaussianBlur(cv_gray, cv_gauss, cv::Size(ksize,ksize), 5,5);
    cv::Mat cv_thresh; cv::threshold(cv_gray, cv_thresh, thresh, 255, cv::THRESH_TOZERO);
    cv::Mat cv_canny; cv::Canny(cv_thresh, cv_canny, thresh/2, thresh);
    cv::HoughCircles(cv_thresh, centroids, CV_HOUGH_GRADIENT, 2, eyeRect.size.height/4,thresh,20 ,eyeRect.size.width/8,eyeRect.size.width/5);
    cv::Mat cv_new_gray;
    cv::cvtColor(cv_canny, cv_new_gray, CV_GRAY2BGR);
    
    CGPoint centroid = CGPointMake(0, 0);

    if(centroids.size()>0){
        size_t minIndex = 0;
        int minRad = cvRound(centroids[0][2]);
        for(size_t i = 0; i < centroids.size(); i++ )
        {
            cv::Point center(cvRound(centroids[i][0]), cvRound(centroids[i][1]));
            int radius = cvRound(centroids[i][2]);
            // circle center
            circle(cv_new_gray, center, 3, cv::Scalar(255,0,0),1,8,0);
            // circle outline
            circle(cv_new_gray, center, radius, cv::Scalar(0,0,255), 1, 8, 0 );
            if(radius < minRad){
                minIndex = i;
            }
        }
        
        //NSLog(@"size: %d", (int)centroids.size());
        
        centroid.x = (CGFloat)centroids[minIndex][0];
        centroid.y = (CGFloat)centroids[minIndex][1];
        //NSLog(@"centroid: %zd", centroid.x);
        cvFrame.x = centroid.x;
        cvFrame.y = centroid.y;
    }else{
        centroid.x = cvFrame.x;
        centroid.y = cvFrame.y;
    }

    UIImage* testImage = [self UIImageFromCVMat:cv_new_gray];
    UIImageView *testView = [[UIImageView alloc] initWithImage:testImage];
    if(side == 0){
        [self.leftEyeTest addSubview:testView];
    }else{
        [self.rightEyeTest addSubview:testView];
    }
    
    CGImageRelease(imageRef);
    return centroid;
}


- (void)drawFaces:(NSArray *)features
      sourceImage:(CIImage *) ciImage
{
    NSArray *sublayers = [NSArray arrayWithArray:[self.previewLayer sublayers]];
    NSInteger sublayersCount = [sublayers count], currentSublayer = 0;
    NSInteger featuresCount = [features count];
    //NSLog(@"feature count: %zd", featuresCount);
    
    UIView *viewToRemove = [self.imagePreview viewWithTag:12];
    [viewToRemove removeFromSuperview];
    UIView *viewToRemove2 = [self.imagePreview viewWithTag:11];
    [viewToRemove2 removeFromSuperview];
    UIView *viewTORemove3 = [self.view viewWithTag:10];
    [viewTORemove3 removeFromSuperview];
    
    [CATransaction begin];
    [CATransaction setValue:(id)kCFBooleanTrue forKey:kCATransactionDisableActions];
    
    // hide all the face layers
    for ( CALayer *layer in sublayers ) {
        if ( [[layer name] isEqualToString:@"FaceLayer"]||
             [[layer name] isEqualToString:@"LeftEyeLayer"]||
             [[layer name] isEqualToString:@"RightEyeLayer"]||
             [[layer name] isEqualToString:@"LeftCentroidLayer"]||
             [[layer name] isEqualToString:@"RightCentroidLayer"])
            [layer setHidden:YES];
    }
    
    if ( featuresCount == 0 ) {
        [CATransaction commit];
        return; // early bail.
    }
    
//    NSLog(@"Num features: %i", (int)featuresCount);
    for ( CIFaceFeature *ff in features ) {

        CGRect faceRect = [ff bounds];
        CGSize eyeSize = faceRect.size;
        eyeSize.width = round(eyeSize.width*0.25);
        eyeSize.height = round(eyeSize.height*0.2);
        
        //Left Eye --------------------------------------------------------------------------
        CGPoint leftEyePosition = [ff leftEyePosition];
        CGRect leftEyeRect;
        leftEyeRect.size = eyeSize;
        leftEyeRect.origin.x = leftEyePosition.x-eyeSize.width/2;
        leftEyeRect.origin.y = leftEyePosition.y-eyeSize.height/2;
        CGPoint leftEyeCentroid = [self getCentroid:leftEyeRect
                                      originalImage:ciImage
                             setCentroidInCVFrame:leftCentroidCVFrame
                                               leftOrRight:0];
        leftEyeCentroid.x += leftEyeRect.origin.x;
        leftEyeCentroid.y = leftEyeRect.origin.y + leftEyeRect.size.height-leftEyeCentroid.y;
        //NSLog(@"CentroidY %i", (int)leftEyeCentroid.y);
        
        [self getRunningAverage: leftEyeCentroid leftOrRight:0];
        
//        UILabel *leftLabel=[ [UILabel alloc] initWithFrame:CGRectMake(10,900,640,20)];
//        leftLabel.text=[NSString stringWithFormat:@"Left Centroid %@",NSStringFromCGPoint(leftAvg)];
//        leftLabel.textColor = [UIColor redColor];
//        leftLabel.tag = 12;
//        [self.imagePreview addSubview:leftLabel];
//        leftLabel = nil;
        
        CGRect leftCentroidRect;
        leftCentroidRect.size = CGSizeMake(10, 10);
        leftCentroidRect.origin.x = leftAvg.x - leftCentroidRect.size.width/2;
        leftCentroidRect.origin.y = leftAvg.y - leftCentroidRect.size.height/2;
        //------------------------------------------------------------------------------------
        //Right Eye --------------------------------------------------------------------------
        CGPoint rightEyePosition = [ff rightEyePosition];
        CGRect rightEyeRect;
        rightEyeRect.size = eyeSize;
        rightEyeRect.origin.x = rightEyePosition.x-eyeSize.width/2;
        rightEyeRect.origin.y = rightEyePosition.y-eyeSize.height/2;
        CGPoint rightEyeCentroid = [self getCentroid:rightEyeRect
                                      originalImage:ciImage
                                setCentroidInCVFrame:rightCentroidCVFrame
                                                leftOrRight:1];
        rightEyeCentroid.x += rightEyeRect.origin.x;
        rightEyeCentroid.y = rightEyeRect.origin.y + rightEyeRect.size.height-rightEyeCentroid.y;
        //NSLog(@"CentroidY %i", (int)rightEyeCentroid.y);
        
        [self getRunningAverage: rightEyeCentroid leftOrRight:1];
        
//        UILabel *rightLabel=[ [UILabel alloc] initWithFrame:CGRectMake(10,950,640,20)];
//        rightLabel.text=[NSString stringWithFormat:@"Right Centroid %@",NSStringFromCGPoint(rightAvg)];
//        rightLabel.textColor = [UIColor redColor];
//        rightLabel.tag = 11;
//        [self.imagePreview addSubview:rightLabel];
//        rightLabel = nil;
        
        CGRect rightCentroidRect;
        rightCentroidRect.size = CGSizeMake(10, 10);
        rightCentroidRect.origin.x = rightAvg.x - rightCentroidRect.size.width/2;
        rightCentroidRect.origin.y = rightAvg.y - rightCentroidRect.size.height/2;
        
        
        if(state == 1){
            [self runPattern];
            [self checkPattern];
        }
        
//        [self createConsensusGaze: leftEyeRect
//                  leftEyeCentroid:leftEyeCentroid
//                     rightEyeRect:rightEyeRect
//                 rightEyeCentroid:rightEyeCentroid];

        //NSLog(@"previewBox frame: %@", NSStringFromCGRect(rightEyeRect));
        //NSLog(@"faceRect frame: %@", NSStringFromCGRect(faceRect));
        
        faceRect.origin.y = _cameraHeight-faceRect.size.height-(faceRect.origin.y);
        leftEyeRect.origin.y = _cameraHeight-leftEyeRect.origin.y-leftEyeRect.size.height;
        leftCentroidRect.origin.y = _cameraHeight-leftCentroidRect.origin.y-leftCentroidRect.size.height;
        rightEyeRect.origin.y = _cameraHeight-rightEyeRect.origin.y-rightEyeRect.size.height;
        rightCentroidRect.origin.y = _cameraHeight-rightCentroidRect.origin.y-rightCentroidRect.size.height;
        
        //Scale everything by screen size
        CGFloat widthScaleBy = screenWidth / _cameraWidth;
        CGFloat heightScaleBy = screenHeight / _cameraHeight;
        [self CGRectScale:faceRect byWidthRatio:widthScaleBy byHeightRatio:heightScaleBy];
        [self CGRectScale:leftEyeRect byWidthRatio:widthScaleBy byHeightRatio:heightScaleBy];
        [self CGRectScale:leftCentroidRect byWidthRatio:widthScaleBy byHeightRatio:heightScaleBy];
        [self CGRectScale:rightEyeRect byWidthRatio:widthScaleBy byHeightRatio:heightScaleBy];
        [self CGRectScale:rightCentroidRect byWidthRatio:widthScaleBy byHeightRatio:heightScaleBy];

        CALayer *featureLayer = nil;
        CALayer *leftEyeLayer = nil;
        CALayer *rightEyeLayer = nil;
        CALayer *leftCentroidLayer = nil;
        CALayer *rightCentroidLayer = nil;
        
        // re-use an existing layer if possible

        while ( (!featureLayer || !leftEyeLayer || !rightEyeLayer || !leftCentroidLayer || !rightCentroidLayer) && (currentSublayer < sublayersCount) ) {
            CALayer *currentLayer = [sublayers objectAtIndex:currentSublayer++];
            if ( [[currentLayer name] isEqualToString:@"FaceLayer"] ) {
                featureLayer = currentLayer;
                [currentLayer setHidden:NO];
            }else if([[currentLayer name] isEqualToString:@"LeftEyeLayer"] ) {
                leftEyeLayer = currentLayer;
                [currentLayer setHidden:NO];
            }else if([[currentLayer name] isEqualToString:@"RightEyeLayer"] ) {
                rightEyeLayer = currentLayer;
                [currentLayer setHidden:NO];
            }else if([[currentLayer name] isEqualToString:@"LeftCentroidLayer"]){
                leftCentroidLayer = currentLayer;
                [currentLayer setHidden:NO];
            }else if([[currentLayer name] isEqualToString:@"RightCentroidLayer"]){
                rightCentroidLayer = currentLayer;
                [currentLayer setHidden:NO];
            }
        }
        
        // create a new one if necessary
        if ( !featureLayer ) {
            featureLayer = [[CALayer alloc]init];
            featureLayer.borderWidth = 2;
            featureLayer.borderColor = [[UIColor redColor] CGColor];
            [featureLayer setName:@"FaceLayer"];
            [self.previewLayer addSublayer:featureLayer];
            featureLayer = nil;
        }
        
        if ( !leftEyeLayer ) {
            leftEyeLayer = [[CALayer alloc]init];
            leftEyeLayer.borderWidth = 2;
            leftEyeLayer.borderColor = [[UIColor redColor] CGColor];
            [leftEyeLayer setName:@"LeftEyeLayer"];
            [self.previewLayer addSublayer:leftEyeLayer];
            leftEyeLayer = nil;
        }
        
        if ( !rightEyeLayer ) {
            rightEyeLayer = [[CALayer alloc]init];
            rightEyeLayer.borderWidth = 2;
            rightEyeLayer.borderColor = [[UIColor redColor] CGColor];
            [rightEyeLayer setName:@"RightEyeLayer"];
            [self.previewLayer addSublayer:rightEyeLayer];
            rightEyeLayer = nil;
        }
        
        if( !leftCentroidLayer) {
            leftCentroidLayer = [[CALayer alloc] init];
            leftCentroidLayer.borderWidth = 1;
            leftCentroidLayer.borderColor = [[UIColor redColor] CGColor];
            [leftCentroidLayer setName:@"LeftCentroidLayer"];
            [self.previewLayer addSublayer:leftCentroidLayer];
            leftCentroidLayer = nil;
        }
        
        if ( !rightCentroidLayer ) {
            rightCentroidLayer = [[CALayer alloc]init];
            rightCentroidLayer.borderWidth = 1;
            rightCentroidLayer.borderColor = [[UIColor redColor] CGColor];
            [rightCentroidLayer setName:@"RightCentroidLayer"];
            [self.previewLayer addSublayer:rightCentroidLayer];
            rightCentroidLayer = nil;
        }
        
        [featureLayer setFrame:faceRect];
        [rightEyeLayer setFrame:rightEyeRect];
        [leftEyeLayer setFrame:leftEyeRect];
        [rightCentroidLayer setFrame:rightCentroidRect];
        [leftCentroidLayer setFrame:leftCentroidRect];
        
        break;
    }
    
    [CATransaction commit];
}

- (void) getRunningAverage: (CGPoint &) eyeCentroid
               leftOrRight: (int) side
{
//    if(side==0){
//        leftAvg.x = eyeCentroid.x;
//        leftAvg.y = eyeCentroid.y;
//    }else{
//        rightAvg.x = eyeCentroid.x;
//        rightAvg.y = eyeCentroid.y;
//    }
    int numOfData = 2;
    if(side == 0){
        if(leftEyeAverage.size() < numOfData){
            leftAvg.x = (leftAvg.x*leftEyeAverage.size()+eyeCentroid.x)/(leftEyeAverage.size()+1);
            leftAvg.y = (leftAvg.y*leftEyeAverage.size()+eyeCentroid.y)/(leftEyeAverage.size()+1);
        }else{
            leftAvg.x = (leftAvg.x*numOfData-leftEyeAverage.at(0).x+eyeCentroid.x)/numOfData;
            leftAvg.y = (leftAvg.y*numOfData-leftEyeAverage.at(0).y+eyeCentroid.y)/numOfData;
            leftEyeAverage.erase(leftEyeAverage.begin());
        }
        leftEyeAverage.push_back(eyeCentroid);
    }else{
        if(rightEyeAverage.size() < numOfData){
            rightAvg.x = (rightAvg.x*rightEyeAverage.size()+eyeCentroid.x)/(rightEyeAverage.size()+1);
            rightAvg.y = (rightAvg.y*rightEyeAverage.size()+eyeCentroid.y)/(rightEyeAverage.size()+1);
        }else{
            rightAvg.x = (rightAvg.x*numOfData-rightEyeAverage.at(0).x+eyeCentroid.x)/numOfData;
            rightAvg.y = (rightAvg.y*numOfData-rightEyeAverage.at(0).y+eyeCentroid.y)/numOfData;
            rightEyeAverage.erase(rightEyeAverage.begin());
        }
        rightEyeAverage.push_back(eyeCentroid);
    }
}

- (void) runPattern
{
    int pixel_threshold = 15;
    if(isCalibrated == false){
        frameCount++;
        if(frameCount>60){
            isCalibrated = true;
            NSLog(@"Calibration Done");
            
            dotArrow2.tag = 3;
            [dotArrow2 setFrame:self.centerDotView.bounds];
            [self.centerDotView addSubview:dotArrow2];
            //dotArrow2 = nil;
            //dotImage2 = nil;
            leftCalibrated = leftAvg;
            rightCalibrated = rightAvg;
            inputPattern+="0";
            frameCount = 0;
            backToCenter = true;
        }
    }else if (leftAvg.x < leftCalibrated.x-pixel_threshold && backToCenter==true){
        
        leftArrow2.tag = 1;
        [leftArrow2 setFrame:self.leftArrowView.bounds];
        [self.leftArrowView addSubview:leftArrow2];
        //leftArrow2 = nil;
        //leftArrowImage2 = nil;
        backToCenter = false;
        inputPattern+="L";
    }else if (rightAvg.x > rightCalibrated.x+pixel_threshold && backToCenter==true){
        
        rightArrow2.tag =2;
        [rightArrow2 setFrame:self.rightArrowView.bounds];
        [self.rightArrowView addSubview:rightArrow2];
        //rightArrow2 = nil;
        //rightArrowImage2 = nil;
        backToCenter = false;
        inputPattern+="R";
    }else if (std::abs(rightAvg.x-rightCalibrated.x) < 5 && std::abs(leftAvg.x-leftCalibrated.x) < 5 && backToCenter==false){
        backToCenter = true;
    }
    UILabel *pattern=[ [UILabel alloc] initWithFrame:CGRectMake(10,10,640,20)];
    pattern.text=[NSString stringWithFormat:@"inputPattern %s",inputPattern.c_str()];
    pattern.textColor = [UIColor redColor];
    pattern.tag = 10;
    [self.view addSubview:pattern];
}

- (void) checkPattern
{
    if(inputPattern.size() >= lockPattern.size()){
        if(lockPattern.compare(inputPattern)==0){
            NSLog(@"CONGRATS!!!!!!!!!!!!!!!");
            [self.unlockedView setHidden:NO];
            [self.unlockedView addSubview:unlockedImageView];
            
        }else{
            NSLog(@"TRY AGAIN");
            NSLog(@"%s",inputPattern.c_str());
            inputPattern.clear();
            isCalibrated = false;
            for(int i = 0; i < inputPattern.size(); i++){
                if(inputPattern.at(i) == '0'){
                    UIView *viewCenterRemove = [self.centerDotView viewWithTag:3];
                    [viewCenterRemove removeFromSuperview];
                }else if(inputPattern.at(i) == 'L'){
                    UIView *viewLeftRemove = [self.leftArrowView viewWithTag:1];
                    [viewLeftRemove removeFromSuperview];
                }else if(inputPattern.at(i) == 'R'){
                    UIView *viewRightRemove = [self.rightArrowView viewWithTag:2];
                    [viewRightRemove removeFromSuperview];
                }
            }
        }
    }
}


- (void) initializePattern{
    NSString *leftFilePath = [[NSBundle mainBundle] pathForResource:@"images/leftArrow" ofType:@"png"];
    UIImage *leftArrowImage = [UIImage imageWithContentsOfFile:leftFilePath];
    UIImageView *leftArrow = [[UIImageView alloc] initWithImage:leftArrowImage];
    [leftArrow setFrame:self.leftArrowView.bounds];
    [self.leftArrowView addSubview:leftArrow];
    leftArrow = nil;
    leftArrowImage = nil;
    
    NSString *rightFilePath = [[NSBundle mainBundle] pathForResource:@"images/rightArrow" ofType:@"png"];
    UIImage *rightArrowImage = [UIImage imageWithContentsOfFile:rightFilePath];
    UIImageView *rightArrow = [[UIImageView alloc] initWithImage:rightArrowImage];
    [rightArrow setFrame:self.rightArrowView.bounds];
    [self.rightArrowView addSubview:rightArrow];
    rightArrow = nil;
    rightArrowImage = nil;
    
    NSString *upFilePath = [[NSBundle mainBundle] pathForResource:@"images/upArrow" ofType:@"png"];
    UIImage *upArrowImage = [UIImage imageWithContentsOfFile:upFilePath];
    UIImageView *upArrow = [[UIImageView alloc] initWithImage:upArrowImage];
    [upArrow setFrame:self.upArrowView.bounds];
    [self.upArrowView addSubview:upArrow];
    upArrow = nil;
    upArrowImage = nil;
    
    NSString *downFilePath = [[NSBundle mainBundle] pathForResource:@"images/downArrow" ofType:@"png"];
    UIImage *downArrowImage = [UIImage imageWithContentsOfFile:downFilePath];
    UIImageView *downArrow = [[UIImageView alloc] initWithImage:downArrowImage];
    [downArrow setFrame:self.downArrowView.bounds];
    [self.downArrowView addSubview:downArrow];
    downArrow = nil;
    downArrowImage = nil;
    
    NSString *dotFilePath = [[NSBundle mainBundle] pathForResource:@"images/dot" ofType:@"png"];
    UIImage *dotImage = [UIImage imageWithContentsOfFile:dotFilePath];
    UIImageView *dotArrow = [[UIImageView alloc] initWithImage:dotImage];
    [dotArrow setFrame:self.centerDotView.bounds];
    [self.centerDotView addSubview:dotArrow];
    dotArrow = nil;
    dotImage = nil;

}

- (void) initializeCamera {
    
    session = [[AVCaptureSession alloc] init];
    session.sessionPreset = _defaultAVCaptureSessionPreset;
    
    self.previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:session];
    self.previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill; //AVLayerVideoGravityResizeAspectFill
    
    self.previewLayer.frame = self.imagePreview.bounds;
    [self.imagePreview.layer addSublayer:self.previewLayer];
//    UIImage *testImage = [UIImage imageNamed:@"right.jpg"];
//    UIImage *testImage = [UIImage imageNamed:@"test.jpg"];

//    if(testImage == nil){
//        NSLog(@"STOPPPPP NO IMAGE");
//    }
//    UIImageView *testImageView = [[UIImageView alloc] initWithImage:testImage];
//    testImageView.frame = self.imagePreview.bounds;
//    [self.imagePreview addSubview:testImageView];
    
    
    UIView *view = [self imagePreview];
    CALayer *viewLayer = [view layer];
    [viewLayer setMasksToBounds:YES];
    
    CGRect bounds = [view bounds];
    [self.previewLayer setFrame:bounds];
    
    NSArray *devices = [AVCaptureDevice devices];
    AVCaptureDevice *frontCamera;
    AVCaptureDevice *backCamera;
    
    for (AVCaptureDevice *device in devices) {
        
        //NSLog(@"Device name: %@", [device localizedName]);
        
        if ([device hasMediaType:AVMediaTypeVideo]) {
            
            if ([device position] == AVCaptureDevicePositionBack) {
                //NSLog(@"Device position : back");
                backCamera = device;
            }
            else {
                //NSLog(@"Device position : front");
                frontCamera = device;
            }
        }
    }
    
    NSError *error = nil;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:frontCamera error:&error];
    if (!input) {
        NSLog(@"ERROR: trying to open camera: %@", error);
    }
    [session addInput:input];
    
    // Create a VideoDataOutput and add it to the session
    videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    
    NSDictionary *rgbOutputSettings =
    [NSDictionary dictionaryWithObject:
     [NSNumber numberWithInt:kCMPixelFormat_32BGRA]
                                forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    
    [videoOutput setVideoSettings:rgbOutputSettings];
    [videoOutput setAlwaysDiscardsLateVideoFrames:YES]; // discard if the data output queue is blocked (as we process the still image)
    
    // Configure your output.
    videoQueue = dispatch_queue_create("myQueue", DISPATCH_QUEUE_SERIAL);
    [videoOutput setSampleBufferDelegate:self queue:videoQueue];
    //dispatch_release(videoQueue);
    
    [session addOutput:videoOutput];
    
    // Start the session running to start the flow of data
    [session startRunning];
}


// Scaling CGRect
- (void)CGRectScale:(CGRect&) rect
       byWidthRatio:(CGFloat&) widthRatio
       byHeightRatio:(CGFloat&) heightRatio
{
    rect.origin.x *= widthRatio;
    rect.origin.y *= heightRatio;
    rect.size.width *= widthRatio;
    rect.size.height *= heightRatio;
}
// Member functions for converting from cvMat to UIImage
- (cv::Mat)cvMatFromUIImage:(UIImage *)image
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = image.size.width;
    CGFloat rows = image.size.height;
    
    cv::Mat cvMat(rows, cols, CV_8UC4); // 8 bits per component, 4 channels (color channels + alpha)
    
    CGContextRef contextRef = CGBitmapContextCreate(cvMat.data,                 // Pointer to  data
                                                    cols,                       // Width of bitmap
                                                    rows,                       // Height of bitmap
                                                    8,                          // Bits per component
                                                    cvMat.step[0],              // Bytes per row
                                                    colorSpace,                 // Colorspace
                                                    kCGImageAlphaNoneSkipLast |
                                                    kCGBitmapByteOrderDefault); // Bitmap info flags
    
    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    CGContextRelease(contextRef);
    
    return cvMat;
}
// Member functions for converting from UIImage to cvMat
-(UIImage *)UIImageFromCVMat:(cv::Mat)cvMat
{
    NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
    CGColorSpaceRef colorSpace;
    
    if (cvMat.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
                                        cvMat.rows,                                 //height
                                        8,                                          //bits per component
                                        8 * cvMat.elemSize(),                       //bits per pixel
                                        cvMat.step[0],                            //bytesPerRow
                                        colorSpace,                                 //colorspace
                                        kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                        provider,                                   //CGDataProviderRef
                                        NULL,                                       //decode
                                        false,                                      //should interpolate
                                        kCGRenderingIntentDefault                   //intent
                                        );
    
    
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return finalImage;
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
