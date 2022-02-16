#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetCircleResolution(60);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	// 配色デザイン P192 ソフトピンク
	vector<ofColor> colors;
	colors.push_back(ofColor(221, 155, 165));
	colors.push_back(ofColor(224, 186, 212));
	colors.push_back(ofColor(229, 181, 161));
	colors.push_back(ofColor(208, 130, 139));
	colors.push_back(ofColor(202, 72, 90));
	colors.push_back(ofColor(163, 150, 197));
	colors.push_back(ofColor(184, 145, 26));
	colors.push_back(ofColor(250, 241, 239));

	int span = 144;
	for (int x = span; x <= 720 - span; x += span) {

		for (int y = span; y <= 720 - span; y += span) {

			ofPushMatrix();
			ofTranslate(x, y);

			int color_1_index = ofRandom(colors.size());
			int color_2_index = (color_1_index + (int)ofRandom(1, colors.size() - 1)) % colors.size();
			int color_3_index = (color_2_index + (int)ofRandom(1, colors.size() - 1)) % colors.size();

			ofFill();
			ofSetColor(colors[color_1_index]);
			ofDrawCircle(glm::vec3(), 58);

			ofNoFill();
			ofSetColor(colors[color_2_index]);
			ofDrawCircle(glm::vec3(), 58);

			for (auto head_size = 4; head_size <= 9; head_size += 5) {

				if (head_size == 4) {

					ofSetColor(colors[color_2_index]);
				}
				else {

					ofSetColor(colors[color_3_index]);
				}

				ofMesh mesh;
				vector<glm::vec3> right, left, frame;
				auto noise_seed_deg = ofRandom(1000);
				auto noise_seed_radius = ofRandom(1000);

				glm::vec3 last_location;
				float last_theta, last_radius;

				for (int i = 0; i < 25; i++) {

					auto noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i) * 0.005), 0, 1, -360, 360);
					auto noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i) * 0.01), 0, 1, -50, 50);
					auto next_noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -360, 360);
					auto next_noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i + 1) * 0.01), 0, 1, -50, 50);

					auto location = glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0);
					auto next = glm::vec3(next_noise_radius * cos(next_noise_deg * DEG_TO_RAD), next_noise_radius * sin(next_noise_deg * DEG_TO_RAD), 0);

					auto direction = next - location;
					auto theta = atan2(direction.y, direction.x);

					right.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta + PI * 0.5), 0));
					left.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta - PI * 0.5), 0));

					last_location = location;
					last_theta = theta;
					last_radius = noise_radius;
				}

				for (int i = 0; i < right.size(); i++) {

					mesh.addVertex(left[i]);
					mesh.addVertex(right[i]);
				}

				for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

					mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
					mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
				}

				auto tmp_head_size = ofMap(mesh.getNumVertices() - 2, 0, mesh.getNumVertices(), 0, head_size);

				mesh.addVertex(last_location);
				int index = mesh.getNumVertices();
				for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

					mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
					frame.push_back(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
				}

				for (int i = index; i < mesh.getNumVertices() - 1; i++) {

					mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
				}

				int span = ofRandom(3, 6);
				int deg_span = 360 / span;
				for (int deg = 0; deg < 360; deg += 30) {

					ofRotate(deg_span);
					mesh.draw();
				}
			}

			ofPopMatrix();
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}