# Gaze-Detection-in-iOS-for-Mobile-Application

## Summary

The purpose of this project is to seek and evaluate methods in using computer vision to determine direction of eye gaze in real time on a mobile computing platform.  The project seeks to employ OpenGL libraries to create 3D face models and to use OpenCV to identify and track movements of the eyeball in relation to the 3D face models.

## Background

The project is fundamentally based on previous work done by Tom Heyman, Vincent Spruty, and Alessandro Ledda in "3D Face Tracking and Gaze Estimation Using a Monocular Camera".  This project seeks to utilize the accessbility of a built-in camera to perform the work in a mobile setting, and the project should be easily extendable to other unique mobile applications such as using eye gaze to take pictures or scroll pages on the screen, for example.

## Challenges

The biggest challenges in this problem are to create an application using only a monocular camera.  Previous works in this field has been done using external references such as specialized glasses or customized headset to use as reference.  The challenge here is to accurately incorporate the OpenGL ES models of 3D face recognition as reference for the position of the eyeballs.  The main challenge for me personally are two parts:  The first challenge is to learn about how to use OpenGL ES and how to properly use its existing libraries to create 3D face models.  The second part is how to use the 3D face models as a reference for eye tracking.  For this project, I am interested in learnign more about OpenGL ES and its application in mobile computer vision applications.  In addition, I am looking to learn about techniques in creating a practical computer vision application that can be easily incorporated in other mobile applications.

## Goals & Deliverables

For this projects, the goals include
1. Create a 3D face model using OpenGL ES
2. Create an eye gaze estimation of an individual based on the 3D face model

The primary success metrics of this project is to demo a working App.  The app should be able to create a 3D model first, then identify the eyeballs, and finally create a gaze estimation in real time.

The task would be quite challenging given the time allotted.  However, I am planning on using any existing libraries available that can provide speedup and scope simplification.

## Schedule

Week of 11/8: Investigate use of OpenGL ES libraries
Week of 11/15: Work on 3D face modeling
Week of 11/22: Work on eye detection
Week of 11/29: Create gaze estimation based on 3D face modeling and eye detection
Week of 12/6: Work on final system tuning and paper presentation