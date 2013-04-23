//
//  LaserPointer
//  opencvExample
//
//  Created by Oriol Ferrer Mesià on 22/04/13.
//
//

#ifndef __opencvExample__LaserPointer__
#define __opencvExample__LaserPointer__

#include <iostream>

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxSimpleGuiToo.h"
#include "ofxTimeMeasurements.h"
#include "ofxBlobTracker.h"

#define PROCESSING_W	640
#define PROCESSING_H	480



class LaserPointer{

public:

	LaserPointer();

	void setup(ofRectangle outputArea, ofBaseApp * app, int camW, int camH, ofImageType type);
	void update(unsigned char* pixels);
	
	void _keyPressed(ofKeyEventArgs &e);
	void _draw(ofEventArgs &e);

	void draw();


	int w, h;
	ofRectangle			canvas;
	ofImageType			imgType;
	bool				showCamImage;

	int					imgThreshold;
	bool				doWrap;
	ofPoint             warpPoints[4];
	ofPoint             imgPoints[4];

	int					numErode;
	int					numDilate;

	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage		grayImg;

	ofxCvGrayscaleImage 	warpImg;
	ofxCvGrayscaleImage 	procImg;
	ofxCvGrayscaleImage 	procThresholded;

	//ofxCvContourFinder		contourFinder;
	ofxBlobTracker          blobTracker;

	ofBaseApp				* app;

    void blobAdded(ofxBlob &_blob);
    void blobMoved(ofxBlob &_blob);
    void blobDeleted(ofxBlob &_blob);


};

#endif /* defined(__opencvExample__File__) */
