#include "testApp.h"
#define VM_DEVELOPMENT 1

//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	messagesSent = 0;
#if VM_DEVELOPMENT
	kinect.initSensor();
	kinect.initColorStream(false);
	kinect.initDepthStream(true);
	//kinect.initBodyIndexStream();
	kinect.initSkeletonStream(false);
	//kinect.setUseTexture(true);
	//simple start
	kinect.start();
#endif

	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	sender.setup("127.0.0.1", 3001);
	//sender.setup("192.168.225.158", 8000); // Sundar
	//sender.setup("192.168.225.160", 8000); // Dalma
	//sender.setup("192.168.225.164", 8000); // Alex

}


const string trackingStateNames[] = {
	"untracked",
	"inferred",
	"tracked"
};


const string handStateNames[] = {
	"unknown",
	"nottracked",
	"open",
	"closed",
	"lasso"
};


const string jointNames[] = {
	"spinebase",
	"spinemid",
	"neck",
	"head",
	"shoulderleft",
	"elbowleft",
	"wristleft",
	"handleft",
	"shoulderright",
	"elbowright",
	"wristright",
	"handright",
	"hipleft",
	"kneeleft",
	"ankleleft",
	"footleft",
	"hipright",
	"kneeright",
	"ankleright",
	"footright",
	"spineshoulder",
	"handtipleft",
	"thumbleft",
	"handtipright",
	"thumbright"
};

//--------------------------------------------------------------
void testApp::update() {
#if VM_DEVELOPMENT
	kinect.update();

	const bool USE_BUNDLE = false;

	// Broadcast OSC
	if (kinect.isFrameNew()) {
		// Bundles just seem to be structural... to the receiver, they remain separate messages
		vector<Kv2Skeleton> skeletons = kinect.getSkeletons();

		// Iterate through skeletons
		ofxOscBundle skeletonsBundle; // note the plural!

		int skeletonIndex = 0;
		//for (vector<Kv2Skeleton>::iterator it = skeletons.begin(); it != skeletons.end(); ++it) {
		//	Kv2Skeleton skeleton = *it;

		for (int i = 0; i < 6; i++) {
			Kv2Skeleton skeleton = skeletons[skeletonIndex];

			ofxOscBundle skeletonBundle;

			ofxOscMessage wholeSkeletonTrackingStatusMessage;
			wholeSkeletonTrackingStatusMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/tracked/"); // set next

			if (skeleton.tracked) {
				// Iterate through joints
				wholeSkeletonTrackingStatusMessage.addStringArg("yes");
				USE_BUNDLE ? skeletonBundle.addMessage(wholeSkeletonTrackingStatusMessage) : sender.sendMessage(wholeSkeletonTrackingStatusMessage);
				messagesSent++;

				// Hands
				// Get hand state
				ofxOscMessage leftHandStateMessage;
				leftHandStateMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/hands/left/");
				leftHandStateMessage.addStringArg(handStateNames[skeleton.leftHandState]);
				USE_BUNDLE ? skeletonBundle.addMessage(leftHandStateMessage) : sender.sendMessage(leftHandStateMessage);
				messagesSent++;

				ofxOscMessage rightHandStateMessage;
				rightHandStateMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/hands/right/");
				rightHandStateMessage.addStringArg(handStateNames[skeleton.rightHandState]);
				USE_BUNDLE ? skeletonBundle.addMessage(rightHandStateMessage) : sender.sendMessage(rightHandStateMessage);
				messagesSent++;

				//for (map<JointType, Kv2Joint>::iterator it = skeleton.joints.begin(); it != skeleton.joints.end(); ++it) {
				//	JointType jointType = it->first;
				//	Kv2Joint joint = it->second;

				for (int j = 0; j < JointType_Count; j++) {
					JointType jointType = (JointType)j;
					Kv2Joint joint = skeleton.joints[jointType];

					// Get position
					ofxOscMessage jointPositionMessage;
					jointPositionMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/joints/" + jointNames[jointType] + "/position"); // position is X Y Z
					jointPositionMessage.addFloatArg(joint.getPosition().x);
					jointPositionMessage.addFloatArg(joint.getPosition().y);
					jointPositionMessage.addFloatArg(joint.getPosition().z);
					USE_BUNDLE ? skeletonBundle.addMessage(jointPositionMessage) : sender.sendMessage(jointPositionMessage);
					messagesSent++;

					// Get orientation
					ofxOscMessage jointOrientationMessage;
					jointOrientationMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/joints/" + jointNames[jointType] + "/orientation"); // quaternion is X Y Z W
					jointOrientationMessage.addFloatArg(joint.getOrientation().asVec4().x);
					jointOrientationMessage.addFloatArg(joint.getOrientation().asVec4().y);
					jointOrientationMessage.addFloatArg(joint.getOrientation().asVec4().z);
					jointOrientationMessage.addFloatArg(joint.getOrientation().asVec4().w);
					USE_BUNDLE ? skeletonBundle.addMessage(jointOrientationMessage) : sender.sendMessage(jointOrientationMessage);
					messagesSent++;

					ofxOscMessage jointTrackingStateMessage;
					jointTrackingStateMessage.setAddress("/skeletons/" + ofToString(skeletonIndex) + "/joints/" + jointNames[jointType] + "/tracking"); // string state
					
					// validation to sidestep some issues here with bogus tracking stats... 
					int trackingState = ofClamp(joint.getTrackingState(), 0, 2);
					jointTrackingStateMessage.addStringArg(trackingStateNames[trackingState]);
					USE_BUNDLE ? skeletonBundle.addMessage(jointTrackingStateMessage) : sender.sendMessage(jointTrackingStateMessage);
					messagesSent++;
				}
				if (USE_BUNDLE) skeletonsBundle.addBundle(skeletonBundle);
			}
			else{
				// Not tracked!
				wholeSkeletonTrackingStatusMessage.addStringArg("no");
				USE_BUNDLE ? skeletonBundle.addMessage(wholeSkeletonTrackingStatusMessage) : sender.sendMessage(wholeSkeletonTrackingStatusMessage);
				messagesSent++;
			}
			skeletonIndex++;
		}
		//ofLog(OF_LOG_VERBOSE, "--- preparing to send ---");
		//ofLog(OF_LOG_VERBOSE, "bundles in skeletons bundle: " + ofToString(skeletonsBundle.getBundleCount()));
		//ofLog(OF_LOG_VERBOSE, "messages in skeletons bundle: " + ofToString(skeletonsBundle.getMessageCount()));
		
		if (USE_BUNDLE) sender.sendBundle(skeletonsBundle);
	}
#endif		

	ofLog(OF_LOG_VERBOSE, "messages sent: " + ofToString(messagesSent));

}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 255);
	
#if VM_DEVELOPMENT
	kinect.draw(0,0);
	kinect.drawDepth(0, 0);
	//kinect.drawBodyIndex(500, 0);
	kinect.drawAllSkeletons(ofVec2f(640,480));
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 't' || key == 'T'){
		ofLog(OF_LOG_NOTICE, "Sending OSC test message");
		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(1);
		m.addFloatArg(3.5f);
		m.addStringArg("hello");
		m.addFloatArg(ofGetElapsedTimef());
		sender.sendMessage(m);
		messagesSent++;
	}
	else if (key == 'b' || key == 'B'){
		ofLog(OF_LOG_NOTICE, "Sending OSC bundle test message");
		ofxOscBundle bundle;

		ofxOscMessage m;
		m.setAddress("/test");
		m.addIntArg(1);
		m.addFloatArg(3.5f);
		m.addStringArg("hello");
		m.addFloatArg(ofGetElapsedTimef());

		bundle.addMessage(m);
		messagesSent++;

		sender.sendBundle(bundle);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


void testApp::exit(){
	ofLog(OF_LOG_VERBOSE, "Exiting App");
	kinect.stop();
}