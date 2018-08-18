#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(ofColor::black);
  
  // Light setup.
  light.setup();
  light.setPosition(0, 0, 200);
  light.setDiffuseColor(ofFloatColor::gold);
  light.setSpecularColor(ofFloatColor::greenYellow);
  ofEnableDepthTest();
  
  // Setup buffer and shader.
  setupShaderBuffer();
}

//--------------------------------------------------------------
void ofApp::update(){
  if (coins.size() > 0) {
    updateCoins();
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
  cam.begin();
    if (coins.size() > 0) {
      shader.begin();
        cylinderMesh.drawInstanced(OF_MESH_FILL, coinMatrices.size());
      shader.end();
    }
  cam.end();
}

void ofApp::exit(){
  // Deallocate the heap.
  for (int i = 0; i < coins.size(); i++) {
    delete coins[i];
  }
}

void ofApp::createCoin(int i) {
  // Update a coin at a specific index, else push this new coin inside the array.
  if (i != -1) {
      coins[i]->resetTransform();
      coins[i]->setPosition(glm::vec3(0, 0, 0));
      coins[i]->velocity = glm::vec3(ofRandom(-2, 2), ofRandom(-2, 2), ofRandom(0, 2));
      coins[i]->life = 1.0; // Reset life.
  } else {
    // Push the coin instance since this vector is being allocated the first time.
    Coin *c = new Coin;
    c->setPosition(glm::vec3(0, 0, 0));
    c->velocity = glm::vec3(ofRandom(-2, 2), ofRandom(-2, 2), ofRandom(0, 2));
    coins.push_back(c);
  }
}

void ofApp::updateCoins() {
  for (int i = 0; i < coins.size(); i++) {
    // Update coin.
    coins[i]->update(ofGetLastFrameTime()/ofRandom(5, 35));
    if (coins[i]->isAlive()) {
      // If coin is alive, we set the matrix with its position.
      coinMatrices[i] = coins[i]->getLocalTransformMatrix();
    } else {
      createCoin(i);
    }
  }
  
  // Finally update buffer data with latest matrices that I want to upload.
  buffer.updateData(0,coinMatrices);
}

void ofApp::setupShaderBuffer() {
  // Allocate vertices equal to the maximum amount of coins that I want to
  // create in the buffer. 
  coinMatrices.resize(maxCoins); // Max number of coins the matrix can hold.
  
  buffer.allocate();
  buffer.bind(GL_TEXTURE_BUFFER);
  buffer.setData(coinMatrices,GL_STREAM_DRAW);

  // Allocate as buffer texture. GL_RGBA32F allows us to get
  // each row of the matrix as a vec4.
  tex.allocateAsBufferTexture(buffer,GL_RGBA32F);

  shader.load("vert.glsl","frag.glsl");
  shader.begin();
  shader.setUniformTexture("tex",tex,0);
  shader.end();

  // Set the geometry which will be drawn as an instance drawing.
  ofCylinderPrimitive cylinder;
  cylinder.set(5, 1);
  cylinder.setResolution(15, 10);
  cylinder.setCylinderColor(ofColor::darkGoldenRod);
  cylinder.setTopCapColor(ofColor::gold);
  cylinder.setBottomCapColor(ofColor::gold);
  cylinderMesh = cylinder.getMesh();
  cylinderMesh.setUsage(GL_STATIC_DRAW);
}

void ofApp::keyPressed(int key) {
  if (coins.size() < maxCoins) {
    for (int i = 0; i < maxCoins; i++) {
      createCoin(-1);
    }
  }
}
