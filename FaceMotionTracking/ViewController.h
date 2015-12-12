//
//  ViewController.h
//  FaceMotionTracking
//
//  Created by Alvin on 11/30/15.
//  Copyright © 2015 Alvin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVCaptureDevice.h>
#import <AVFoundation/AVCaptureSession.h>
#import <AVFoundation/AVCaptureOutput.h>
#import <CoreVideo/CVPixelBuffer.h>

@interface ViewController : UIViewController<AVCaptureVideoDataOutputSampleBufferDelegate> {
}
@property(nonatomic, retain) AVCaptureVideoDataOutput *videoOutput;
@property (weak, nonatomic) IBOutlet UIView *imagePreview;
@property (weak, nonatomic) IBOutlet UIView *leftEyeTest;
@property (weak, nonatomic) IBOutlet UIView *rightEyeTest;
@property (weak, nonatomic) IBOutlet UIView *unlockedView;
@property (weak, nonatomic) IBOutlet UIImageView *leftArrowView;
@property (weak, nonatomic) IBOutlet UIImageView *rightArrowView;
@property (weak, nonatomic) IBOutlet UIImageView *upArrowView;
@property (weak, nonatomic) IBOutlet UIImageView *downArrowView;
@property (weak, nonatomic) IBOutlet UIImageView *centerDotView;

@end

