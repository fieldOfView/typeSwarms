#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//	ofEnableSmoothing();

	ofBackground(0, 0, 0);
	font.loadFont("helvetica.ttf", 300, true, true, true);

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// instatiates the class particle and send them to their positions
	for (int i = 0; i < 400; i++) {

		particle myParticle;
		myParticle.setInitialCondition(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), 0, 0);
		particles.push_back(myParticle);

	}

	// define repel and radius
	strength = 0.5f;
	radius = 600;
	margin = 20;

	activateDraw = false;
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < particles.size(); i++) {
		particles[i].resetForce();
	}

	for (int i = 0; i < particles.size(); i++) {
		for (int j = 0; j < particles.size(); j++) {
			if (i != j) {
				particles[i].addForFlocking(particles[j]);
			}
		}

		particles[i].addRepulsionForce(mouseX, mouseY, 150, 0.4);
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}

	if (!activateDraw)
		return;

	double scale = (ofGetWidth() - 2 * margin) / (double)boundingBox.width;
	double offset = (ofGetHeight() + (boundingBox.getHeight() * scale)) / 2.;

	for (int i = 0; i < min(points.size(), particles.size()); i++) {
		particles[i].addAttractionForce(
			(points[i].x - boundingBox.getLeft()) * scale + margin, 
			(points[i].y - boundingBox.getBottom()) * scale + offset, 
			radius, strength
		);

		particles[i].update();
	}

}


void ofApp::writeShape() {
	points.clear();

	boundingBox = font.getStringBoundingBox(word, 0, 0);

	double totalLength = 0;

	vector<ofTTFCharacter> characters = font.getStringAsPoints(word);
	for (int m = 0; m < characters.size(); m++) {
		vector<ofPolyline> outlines = characters[m].getOutline();
		for (int n = 0; n < outlines.size(); n++) {
			ofPolyline outline = outlines[n];
			totalLength += outline.getLengthAtIndex(outline.size() - 1);
		}
	}

	double spacing = totalLength / 400.;
	for (int m = 0; m < characters.size(); m++) {
		vector<ofPolyline> outlines = characters[m].getOutline();
		for (int n = 0; n < outlines.size(); n++) {
			vector<ofPoint> resampled = outlines[n].getResampledBySpacing(spacing).getVertices();
			points.insert(points.end(), resampled.begin(), resampled.end());
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw(){

	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	switch (key) {
	case OF_KEY_RETURN:
		word = "";
		activateDraw = false;
		break;

	case OF_KEY_BACKSPACE:
		if (word.size() > 1) {
			word.erase(word.size() - 1);
			writeShape();
		}
		else {
			word = "";
			activateDraw = false;
		}
		break;

	default:
		if (key <= 255) {
			word.push_back(char(key));
			activateDraw = true;
			writeShape();
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	word = "";
	activateDraw = false;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(){

}
