Beyond Reality Face SDK v3.0.16 - Readme

			- ready to try! - 

You can try the BRF SDK free of charge to evaluate if
this SDK fits your needs. Please don't hide the BRF logo!

If you need a logo free version or if you want to use
BRF commercially, just contact us:

http://www.tastenkunst.com/#/contact

Read the EULA carefully before using the SDKs. Once you
decide to use BRF commercially, you will get a seperate license
agreement, that you need to agree with.

			- visit us online - 

+ Facebook: 				https://www.facebook.com/BeyondRealityFace
+ Twitter:	 				https://twitter.com/tastenkunst
+ website:					https://www.beyond-reality-face.com
+ blog!!:					http://blog.tastenkunst.com

			- technical overview -

￼The Beyond Reality Face SDK is a face tracking library,
that is available for developers for the following platforms:

+ Adobe Flash				– browser plugin on Windows/Mac OSX – Chrome/Firefox/IE/Safari etc.
+ Adobe AIR for Desktop		– installable apps for Windows/Mac OSX systems
+ Adobe AIR for Mobile		– installable apps for iOS/Android (using a AIR native extension - ANE)
+ HTML5/Javascript/WebRTC	– face tracking right in your browser (even in Chrome for Android!)
+ Native C++ Library		– BRF NXT native for iOS and (soon) Android developers

And BRF comes with the following usable components:
 
+ Point Tracking			– tracks multiple points in webcam streams.
+ Face Detection			– Finds face rectangles in images/webcam streams.
+ FaceShape Tracking		– analyses a found face in detail using a shape of 68 points.
+ CandideShape Tracking		– used to place 3D objects on the face by updating a Candide 3 model.

			- getting started - 

+ Try the Techdemos:		http://www.tastenkunst.com/#/brf/techdemo
+ Download the SDK:			http://www.tastenkunst.com/#/brf/download
+ Find the API here:		
	as3: https://www.beyond-reality-face.com/api/as3
	cpp: https://www.beyond-reality-face.com/api/cpp

We provide several packages:

+ the SWC package for Adobe Flash and Adobe AIR for Desktop.
+ the ANE package for Adobe AIR for Mobile (iOS/Android)
+ the JS package for HTML (modern browsers, like Chrome and Firefox)
+ the native C++ library for iOS

All packages share the exact same API. 
Don't use the SWC on mobile, use the ANE instead.

			- SDK structure -
			
			- Actionscript -

We develop using FDT (http://www.fdt.powerflasher.com), so the projects are FDT-projects, 
that can easily be imported into your FDT. Use the launcher in /launch/ folder to start the examples.

We also included a BRF_NXT_AS3_EXAMPLES_LAUNCHER.fla file for Flash CC developers.
Keep in mind to use SHIFT + CTRL + ENTER to compile in the Flash IDE to debug the SWF
Or open the /bin/index.html file to view the SWF.

For all other IDEs (e.g. Flash Builder, FlashDevelop, Intellij) the classpath settings
are as follows:
+ /src/						- the example source code 
+ /lib/						- the SWC lib folder, add all included SWCs to your classpath

What examples are included:
+ ExamplePointTracking		- Shows how to track points in webcam streams.
+ ExampleFaceDetection		- Shows how to detect faces and mark them as rectangles.
+ ExampleFaceTracking		- Shows how to detect faces and analyse them to get a shape with 68 points.
+ ExampleCandideTracking	- Shows how to (additionally to ExampleFaceTracking) update a 
							  Candide 3 model as well (it is used to update 3D engines).
+ several other Examples, you can always compile classes, that start with Example
							  
We also included a Flare3D v2.7.1 (http://www.flare3d.com) example (ExampleFlare3D)
and an Away3D v4.1.6 (http://www.away3d.com) example (ExampleAway3D).

If you want to use higher camera resolutions, look inside the 
com.tastenkunst.as3.brf.nxt.examples.hires package.

			- Javascript - 
			
The API is exactly the same as AS3. 
We used CreateJS for the examples, because it's a AS3 like API. 
Of course you can use whatever library you want to. 

In the end, BRF needs just a color pixel array (from an image or webcam stream).

You can set the BRF logo whereever you want and at a decent size. 
But please don't hide the logo and its link! Thanks.

+ /bin/index.html							// Load one example from the includes list. For the rest, see the js files.

+ /js/lib/brf								// all three files are necessary, BRF_NXT_public_API.js needs to be loaded before BRF_NXT_v3.x.y_trial.js
+ /js/lib/brf/BRF_NXT_public_API.js			// some classes, that needs to be set up for the examples to work.
+ /js/lib/brf/BRF_NXT_v3.x.y_trial.js		// the actual face tracking library
+ /js/lib/brf/BRF_NXT_v3.x.y_trial.js.mem	// will be loaded by BRF_NXT_v3.x.y_trial.js, see index.html how to set its path

At the moment there is no 3D example, we are working on that.

			- native C++ -
			
+ BRF_NXT_CPP_IOS_EXAMPLES: includes a XCode project, that you can easily open. Connect your iPhone 5 (and newer) and hit the "run" button.

We seperated the BRF logic from the Objectiv-C code as much as possible. Choose one of the 4 examples classes.

			- release notes -
			
v3.0.16 - 20.02.2015:

+ Added (C++): a package for native iOS developers including a static library and some C++ helper classes. 
		Also a XCode project, that will let you play with the default ExampleFaceTrackingIOS with only one click on the "run" button.
		
+ Updated (JS): The emscripten version to 1.29.0.  		
			
v3.0.15 - 25.01.2015:

+ Updated AIR SDK settings for FDT projects to AIR version 16.0 and swf-version 27
+ SWC version is still compiled using playerglobal.swc 11.1, but for AIR projects there is no need to stay that low.

+ Defaulted Face Detection from finding faces from large to small to be finding small to large. This has a performance hit, but is more accurate.

+ Fixed (JS): BRFManager.reset was acutally not working. doh! Now it does

+ Added (ANE): 
		 + iOS 64 bit support (Universal library)
		 + Android x86 support. Could anyone with a x86 Android please test that build and tell me, if it works for you. Thanks guys!

+ Added (SWC, ANE): ExampleSimpleBlinkAndYawyDetection and ExampleSimpleSmileDetection to show some simple approaches for these detections.

+ Added (SWC): ExampleFireBeard. A fun little example with a Flare3D curved plane and perlin noise textures.

+ Added (SWC): facesubstitution package. We had that before in BRF v2.3.x. An exporter and viewer for face substitution textures/uv data.
		 Also added classes for the candide triangle shape, not just the face shape.
		 You need to click on the eyes to do an image face detection/tracking and then click generate mask etc.

v3.0.10 - 16.09.2014:

+ Updated Flex SDK settings for FDT projects to player version 11.8 and swf-version 21 (RectangleTextures)
+ Updated AIR SDK settings for FDT projects to AIR version 14.0 and swf-version 25
+ SWC version is still compiled using playerglobal.swc 11.1, but for AIR projects there is no need to stay that low.

+ Added (SWC, ANE): SWF_Version_Flash_Player_Version.txt for your info about SWF versions and release dates.

+ Added (SWC, ANE, JS): resetMode in BRFManager.setFaceTrackingVars (default: 1)
		 Can be either 0 or 1 (default), where 0 is the old and 1 is the new reset behaviour.
		 The reset behavior is now tighter and will reset more ofter, if the shape doesn't seem to be correct.

+ Added (SWC, ANE, JS): BRFManager.updateByEyes(bmd : BitmapData, leftEye : Point, rightEye : Point, numUpdates : int = 25)
		 Single image face tracking was not working as expected. There was a 50% chance to get a good result, but it was more like a lucky shot. This method lets you specify the left and right eye position. It skips the face detection part and uses the eye positions to align the face tracking shape. This way there is a much better chance to get a good result out of a single image. (Works only with mode == BRFMode.FACE_TRACKING.)
		 The method will also calculate the BRFManager.lastDetectedFace from the eyes, so you can draw it. 

+ Changed (SWC, ANE): Changed the structure of ExampleBase.as to get a better handling for single image tracking.
+ Changed (JS): Changed the examples structure for the javascript SDK a little bit. 
				There is only one index.html left and you need to choose only one of the available example files.

+ Added (SWC, ANE, JS): ExampleBase.mirrored getter and setter
		 Also restructured this class for a better single image handling.
+ Added (SWC, ANE, JS): ExampleFaceDetectionImage to demonstrate automatic face detection on single images.
+ Added (SWC, ANE, JS): ExampleFaceTrackingImage to demonstrate BRFManager.updateByEyes.

To catch up a little with AS3 examples:
+ Added (JS): js/examples/hires/ExamplePointTracking1280x720.js
+ Added (JS): js/examples/hires/ExampleFaceDetection1280x720.js
+ Added (JS): js/examples/hires/ExampleFaceTracking1280x720.js
+ Added (JS): js/examples/hires/ExampleCandideTracking1280x720.js
+ Added (JS): js/examples/specials/ExampleComponentFlow.js

v3.0.6 - 29.08.2014:

+ Here we go: Beyond Reality Face Nxt - Javascript SDK for realtime face tracking right in your browser! 
  getUserMedia (webcam stream) is currently supported in Chrome and Firefox, but not in IE and Safari :/
  But it even works (at a lower framerate) in Chrome for Android (v35+), yeah!
  The JS SDK has the exact same API as the AS3 libs. And thanks to CreateJS the examples look like their AS3 versions as well :).
  But you are free to use whatever JS libs you want, in the end BRF just needs the webcam pixels and some params, nothing more.
+ Fixed the ReferenceError in ANE version

v3.0.2 - 13.06.2014:

+ Huge steps are taken to release the JavaScript Version of the SDK, that's why we need to
  keep all sources in Sync. So this time, minor changes under the hood.
+ Added Away3D v4.1.6 example for Adobe Flash, Adobe AIR for Desktop and Adobe AIR for Mobile

v3.0.1 - 30.05.2014:

OMG. We really made it? I can't believe it! If you see a super happy Flash dev with
a huge smile on his face, that's probably me!

+ Completely rewritten BRF in AS3 and C++
+ Added support for Adobe AIR for Mobile (iOS and Android) using an AIR native extension!
+ Increaded accuracy and flexibility of the face tracking
+ Reduced calculation time and SWC file size
+ Added a Candide 3 model with 6 Action Units (jaw, mouth, brows)
+ Improved 3D object placement using the Candide 3 model with wider angles
+ Input BitmapData can now differ from 640x480 (eg. 480x640, 320x240, 800x600)
+ Added high camera resolution examples
+ Added support for Flare3D v2.7

		- Licenses -

Used Haar Cascade: haarcascade_frontalface_default.xml
		
<!--
    Stump-based 24x24 discrete(?) adaboost frontal face detector.
    Created by Rainer Lienhart.

////////////////////////////////////////////////////////////////////////////////////////

  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

  By downloading, copying, installing or using the software you agree to this license.
  If you do not agree to this license, do not download, install,
  copy or use the software.


                        Intel License Agreement
                For Open Source Computer Vision Library

 Copyright (C) 2000, Intel Corporation, all rights reserved.
 Third party copyrights are property of their respective owners.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

   * Redistribution's of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

   * Redistribution's in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   * The name of Intel Corporation may not be used to endorse or promote products
     derived from this software without specific prior written permission.

 This software is provided by the copyright holders and contributors "as is" and
 any express or implied warranties, including, but not limited to, the implied
 warranties of merchantability and fitness for a particular purpose are disclaimed.
 In no event shall the Intel Corporation or contributors be liable for any direct,
 indirect, incidental, special, exemplary, or consequential damages
 (including, but not limited to, procurement of substitute goods or services;
 loss of use, data, or profits; or business interruption) however caused
 and on any theory of liability, whether in contract, strict liability,
 or tort (including negligence or otherwise) arising in any way out of
 the use of this software, even if advised of the possibility of such damage.
-->
