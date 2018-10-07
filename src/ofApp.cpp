#include "ofApp.h"

int letter = 'a';
//--------------------------------------------------------------
void ofApp::setup(){

	//	ofEnableSmoothing();

	ofBackground(0,0,0);
	testFont.loadFont("helvetica.ttf", 300, true, true, true);

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// instatiates the class particle and send them to their positions
	for (int i = 0; i <400; i++){

		particle myParticle;
		myParticle.setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
		particles.push_back(myParticle);

	}

	// define repel and radius
	bRepel		= true;
	radius		= 100;
	strength	= 0.5f;

	activateDraw = false;
	bCircle = false;
}

//--------------------------------------------------------------
void ofApp::update(){

	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}

	for (int i = 0; i < particles.size(); i++){
		for (int j = 0; j < particles.size(); j++){
			if (i != j){
				particles[i].addForFlocking(particles[j]);
			}
		}

		particles[i].addRepulsionForce(mouseX, mouseY, 150, 0.4);
	}

	for (int i = 0; i < particles.size(); i++){
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}

	writeShape2();

}

void ofApp::writeShape() {

	if(activateDraw == true) {

		ofTTFCharacter testChar;
		testChar = testFont.getCharacterAsPoints(letter);

		int count = 0;
		myStroke.clear();	// clear it out

		for(int k = 0; k < 1; k++){  // only do outside...
			vector<ofPoint> outsidePoints = testChar.getOutline()[0].getVertices();
			if (outsidePoints.size() == 0) {
				break;
			}

			for(int i = 0; i < outsidePoints.size(); i++){

				ptsx = outsidePoints[i].x;
				ptsy = outsidePoints[i].y;

				myStroke.addPoint(ptsx, ptsy);	// add into the stroke object
			}

			ptsx = outsidePoints[0].x;
			ptsy = outsidePoints[0].y;

			myStroke.addPoint(ptsx, ptsy);	   /// add point 0 again (to close the shape)

			//stroke otherStroke;
			//otherStroke.push_back(ptsx);
		}


		myStroke.resample(particles.size());  // resample so that we are now 100 points exactly....


		for (int i = 0; i < particles.size(); i++){
			//particles[i].resetForce();

			particles[i].addAttractionForce( myStroke.pts[i].x + ofGetWidth()/2 - 195 ,  myStroke.pts[i].y + ofGetHeight()/2 + 200, 600, strength);

			particles[i].update();
			//cout << "draw" << activateDraw << endl;
		}
	}

	if( (int)ofGetElapsedTimef() % 10 == 0){

		activateDraw = false;
	}


}


void ofApp::writeShape2() {

	string wordToWrite = Wrd;


	int widthStep = 0;
	if(activateDraw == true) {


		int nLetters = wordToWrite.length();

		// create a temporary array of strok objects.  this wold be better not made temporary...
		int resampleSize = particles.size() / (float)nLetters;
		int particleCounter = 0;

		for (int m = 0; m < nLetters; m++){
			//cout << nLetters;
			ofTTFCharacter testChar;
			testChar = testFont.getCharacterAsPoints(wordToWrite[m]);

			string Str = "";
			Str.push_back(wordToWrite[m]);

			float widthOfCharacter = testFont.getStringBoundingBox(Str, 0,0).width;

			int count = 0;
			myStroke.clear();	// clear it out

			const vector<ofPolyline> outlines = testChar.getOutline();
			if (outlines.size() == 0) {
				continue;
			}
			for (int k = 0; k < 1; k++) {  // only do outside...
				vector<ofPoint> outsidePoints = outlines[k].getVertices();
				if (outsidePoints.size() == 0) {
					continue;
				}
				//cout << testChar.contours[k].pts.size();

				for(int i = 0; i < outsidePoints.size(); i++){

					ptsx = outsidePoints[i].x;
					ptsy = outsidePoints[i].y;

					myStroke.addPoint(ptsx, ptsy);	// add into the stroke object
				}

				ptsx = outsidePoints[0].x;
				ptsy = outsidePoints[0].y;

				myStroke.addPoint(ptsx, ptsy);	   /// add point 0 again (to close the shape)

				//stroke otherStroke;
				//otherStroke.push_back(ptsx);

			}

			if (myStroke.getLength() == 0) {
				continue;
			}

			myStroke.resample(resampleSize);  // resample so that we are now 100 points exactly....

			for (int i = 0; i < myStroke.pts.size(); i++){
				//particles[i].resetForce();
				//
				particles[particleCounter].addAttractionForce( myStroke.pts[i].x + ofGetWidth()/8 + widthStep ,  myStroke.pts[i].y + ofGetHeight()/2 - testFont.getStringBoundingBox(Str, 0,0).height/2 + 300 , 600, strength);

				particles[particleCounter].update();
				particleCounter ++;
				//cout << "draw" << activateDraw << endl;


			}

			widthStep += widthOfCharacter;

		}

	}

	//	if( (int)ofGetElapsedTimef() % 10 == 0){

	//		activateDraw = false;
	//	}



}


//--------------------------------------------------------------
void ofApp::draw(){

	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	if(key <= 255) {
		Wrd.push_back(char(key));
		activateDraw = true;
	}

	if(key == OF_KEY_RETURN)  {
		Wrd = "";
		activateDraw = false;
	}
	if(key == OF_KEY_BACKSPACE)  {
		Wrd = "";
		activateDraw = false;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	//particles.erase(particles.begin());
	//particle myParticle;
	//myParticle.setInitialCondition(x,y,0,0);
	//particles.push_back(myParticle);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	Wrd = "";
	activateDraw = false;
	activateLoop = true;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(){
}
