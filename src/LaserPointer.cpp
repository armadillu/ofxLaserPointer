//
//  File.cpp
//  opencvExample
//
//  Created by Oriol Ferrer Mesià on 22/04/13.
//
//

#include "LaserPointer.h"

LaserPointer::LaserPointer(){

}

void LaserPointer::setup(ofRectangle outputArea, ofBaseApp * app_, int camW, int camH, ofImageType type){

	canvas = outputArea;
	app = app_;
	imgType = type;
	w = camW;
	h = camH;

	if (imgType==OF_IMAGE_COLOR){
		colorImg.allocate(w,h);
		//colorImg.setUseTexture(false);
	}
	grayImg.allocate(w,h);
	//grayImg.setUseTexture(false);

	warpImg.allocate(w, h);
	//warpImg.setUseTexture(false);
	procImg.allocate(PROCESSING_W, PROCESSING_H);
	//procImg.setUseTexture(false);
	procThresholded.allocate(PROCESSING_W, PROCESSING_H);
	//procThresholded.setUseTexture(false);

	imgPoints[0].x = warpPoints[0].x = 0;
	imgPoints[0].y = warpPoints[0].y = 0;
	imgPoints[1].x = warpPoints[1].x = w;
	imgPoints[1].y = warpPoints[1].y = 0;
	imgPoints[2].x = warpPoints[2].x = w;
	imgPoints[2].y = warpPoints[2].y = h;
	imgPoints[3].x = warpPoints[3].x = 0;
	imgPoints[3].y = warpPoints[3].y = h;

	ofAddListener(ofEvents().keyPressed, this, &LaserPointer::_keyPressed);
	ofAddListener(ofEvents().draw, this, &LaserPointer::_draw);

	defaultSimpleGuiConfig.gridSize.x = 320 + 22;
	defaultSimpleGuiConfig.sliderHeight = 9;
	defaultSimpleGuiConfig.sliderTextHeight = 20;
	defaultSimpleGuiConfig.slider2DTextHeight = 50;
	defaultSimpleGuiConfig.toggleHeight = 20;
	defaultSimpleGuiConfig.titleHeight = 20;

	defaultSimpleGuiConfig.buttonHeight = 20;
	defaultSimpleGuiConfig.comboBoxHeight = 20;
	defaultSimpleGuiConfig.titleHeight = 20;
	defaultSimpleGuiConfig.offset.set(10,10);
	//defaultSimpleGuiConfig.padding.set(10, 4);
	defaultSimpleGuiConfig.borderColor = 0x000000;
	defaultSimpleGuiConfig.fullActiveColor = 0x009a48;
	defaultSimpleGuiConfig.textColor = 0xFFFFFF;
	defaultSimpleGuiConfig.textOverColor= 0x009a48;

	//GUI //////////////////////////////////////////

	gui.addToggle("doWarp", doWrap );
	gui.addToggle("showCamImage", showCamImage);
	gui.addToggle("drawDebug", drawDebug);

	gui.addTitle("Image Processing").setNewColumn(false);
	gui.addSlider("imgThreshold", imgThreshold, 1, 255 );
	gui.addSlider("numErode", numErode, 0, 4 );
	gui.addSlider("numDilate", numDilate, 0, 4 );

	gui.addQuadWarper("cameraCrop", grayImg, warpPoints);
	gui.addContent("warpedImage", warpImg).setNewColumn(true);
	gui.addContent("thresholded", procThresholded);

	gui.loadFromXML();

	ofAddListener(blobTracker.blobAdded, this, &LaserPointer::blobAdded);
    ofAddListener(blobTracker.blobMoved, this, &LaserPointer::blobMoved);
    ofAddListener(blobTracker.blobDeleted, this, &LaserPointer::blobDeleted);

}


void LaserPointer::update(unsigned char* pixels){

	TIME_SAMPLE_START("u");
	if( imgType == OF_IMAGE_COLOR ){
		colorImg.setFromPixels(pixels, w, h);
		grayImg= colorImg;
	}else{
		grayImg.setFromPixels(pixels, w, h);
	}
	//at this point grayImge has the native reso grayscale pixels

	TIME_SAMPLE_START("warp");
	if (doWrap){
		warpImg.warpIntoMe( grayImg, warpPoints, imgPoints);
	}else{
		warpImg = grayImg;
	}
	TIME_SAMPLE_STOP("warp");


	procImg.scaleIntoMe(warpImg); //downscale to processing reso
	procThresholded = procImg;
	procThresholded.threshold(imgThreshold);

	for(int i=0; i < numErode; i++)
		procThresholded.erode();

	for(int i=0; i < numDilate; i++)
		procThresholded.dilate();

	procThresholded.setUseTexture(true);
	procThresholded.updateTexture();

	//contourFinder.findContours(procThresholded, 1, 120, 5, false); //TODO
	blobTracker.update(procThresholded, imgThreshold, 1/*min area*/);
	
	TIME_SAMPLE_STOP("u");
	
}

void LaserPointer::draw(){

	if(showCamImage){
		warpImg.draw(0,0, canvas.width, canvas.height);
	}


	if (drawDebug){

		ofPushMatrix();
		ofScale( canvas.width / PROCESSING_W, canvas.height / PROCESSING_H);

		blobTracker.draw(0,0);
		for (int i = 0; i < blobTracker.size(); i++){

			ofSetColor(255);
			ofDrawBitmapString(ofToString( blobTracker[i].id ),
							   20 + blobTracker[i].boundingRect.getCenter().x * PROCESSING_W,
							   10 + blobTracker[i].boundingRect.getCenter().y * PROCESSING_H
							   );
		}
		ofPopMatrix();

	}
	//TIME_SAMPLE_DRAW_BOTTOM_RIGHT();
}

void LaserPointer::_keyPressed(ofKeyEventArgs &e){

	if(e.key=='*') gui.toggleDraw();

	if (gui.isOn()) ofShowCursor();
	else ofHideCursor();

}

void LaserPointer::_draw(ofEventArgs &args){	//last thing to draw
	if ( gui.isOn() ){
		ofSetupScreen();
		glLineWidth(1);
		gui.draw();
	}
}

void LaserPointer::blobAdded(ofxBlob &_blob){
    //ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " added" );
	app->mousePressed(canvas.width * _blob.boundingRect.getCenter().x , canvas.height * _blob.boundingRect.getCenter().y , 0);
}

void LaserPointer::blobMoved(ofxBlob &_blob){
    //ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " moved" );
	app->mouseDragged(canvas.width * _blob.boundingRect.getCenter().x , canvas.height * _blob.boundingRect.getCenter().y , 0);
}

void LaserPointer::blobDeleted(ofxBlob &_blob){
    //ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " deleted" );
	app->mouseReleased(canvas.width * _blob.boundingRect.getCenter().x, canvas.height * _blob.boundingRect.getCenter().y , 0);
}

