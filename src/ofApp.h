#pragma once

#include "ofMain.h"

class Coin : public ofNode {
  public:
    void update(float dt) {
      // Update particle props.
      this->setPosition(this->getPosition() + velocity);
      this->rotate(rotation, 1.0, 1.0, 1.0);
      life = life - dt;
    }
  
    bool isAlive() {
      return life >= 0.0;
    }
  
    glm::vec3 velocity;
    float life = 1.0;
    float rotation = ofRandom(0, 2);
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void exit();
  
    void createCoin(int idx);
    void updateCoins();
    void setupShaderBuffer();
  
    void keyPressed(int key);
  
    ofEasyCam cam;
    ofLight light;
    ofVboMesh cylinderMesh;
  
    // Shader stuff
    ofShader shader;
    ofBufferObject buffer;
    ofTexture tex;
    vector<ofMatrix4x4> coinMatrices;
  
    vector<Coin*> coins;
  
    const int maxCoins = 15000;
};
