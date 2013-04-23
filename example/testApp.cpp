#include "testApp.h"


void testApp::setup(){

	//vidPlayer.loadMovie("laser.mov");
	//vidPlayer.play();


	ofBackground(22);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	remoteCam.initGrabber(640,480);
	remoteCam.start();
	remoteCam.setRequestedImageType(OF_IMAGE_GRAYSCALE);
	remoteCam.setRequestedCompressionQuality( 90 );//jpeg, [0..100]
	laser.setup(ofRectangle(0,0, ofGetWidth(), ofGetHeight()), this, 640, 480, OF_IMAGE_GRAYSCALE);

}


void testApp::update(){

//	vidPlayer.update();
//
//	if (vidPlayer.isFrameNew()){
//		laser.update( vidPlayer.getPixels() );
//	}

	remoteCam.update();
	
	if(remoteCam.isFrameNew()){
		laser.update( remoteCam.getPixels() );
	}

}


void testApp::draw(){

	laser.draw();
	ofSetColor(255,0,0);

	ofNoFill();
	ofBeginShape();
	for(int i = 0; i < line.size(); i++){
		ofVertex(line[i].x, line[i].y);
	}
	ofEndShape();
	ofFill();
}

void testApp::exit(){
	remoteCam.close();
}


void testApp::keyPressed(int key){

	
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

	line.push_back( ofVec2f(x ,y ));
}


void testApp::mousePressed(int x, int y, int button){

	line.push_back( ofVec2f(x ,y ));
}


void testApp::mouseReleased(int x, int y, int button){

	line.push_back( ofVec2f(x ,y ));
}


