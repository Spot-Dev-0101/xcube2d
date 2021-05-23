#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:

		//create the boxes
		RectPhysics box1;
		RectPhysics box2;
		RectPhysics box3;
		RectPhysics box4;
		RectPhysics box5;
		
		std::string gravityStr = "Gravity (space): On";
		std::string collisionStr = "Collision (A): On";

		/* GAMEPLAY */
		int score, numKeys, lives;
		bool gameWon;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();

		//void collided(std::shared_ptr<PhysicsObject>);
	public:
        MyGame();
		~MyGame();
};

#endif