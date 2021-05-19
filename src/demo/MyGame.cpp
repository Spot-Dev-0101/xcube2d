#include "MyGame.h"

std::shared_ptr<void> &collided(std::shared_ptr<PhysicsObject> self, std::shared_ptr<PhysicsObject> other){
	std::cout << self->name << " Collided with " << other->name << "\n";
	return std::shared_ptr<void>();
}

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), box1(10, 500, 30, 30), box2(260, 10, 50, 50), box3(250, 300, 20, 20), box4(500, 300, 40, 40)
, box5(400, 50, 70, 70) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);

	physics->windowSize = gfx->getCurrentWindowSize();

	gfx->useFont(font);
	gfx->setVerticalSync(true);

	//void (*collisionFunction)(std::shared_ptr<PhysicsObject> other) = &collided;

	//Setup all the boxes
	//All have Gravity, collisions, are confined to the screen, and pass through collision detection
	box1.physicsObject->useGravity = true;
	box1.physicsObject->useCollisions = true;
	box1.physicsObject->confineToScreen = true;
	box1.physicsObject->setVelocity(0, 0);
	box1.physicsObject->usePassThroughDetection = true;
	box1.physicsObject->name = "box1";
	box1.physicsObject->setOnCollide(&collided);

	box2.physicsObject->useGravity = true;
	box2.physicsObject->useCollisions = true;
	box2.physicsObject->confineToScreen = true;
	box2.physicsObject->setVelocity(0, 0);
	box2.physicsObject->usePassThroughDetection = true;
	box2.physicsObject->name = "box2";
	box2.physicsObject->setOnCollide(&collided);

	box3.physicsObject->useGravity = true;
	box3.physicsObject->useCollisions = true;
	box3.physicsObject->confineToScreen = true;
	box3.physicsObject->setVelocity(0, 0);
	box3.physicsObject->usePassThroughDetection = true;
	box3.physicsObject->name = "box3";
	box3.physicsObject->setOnCollide(&collided);

	box4.physicsObject->useGravity = true;
	box4.physicsObject->useCollisions = true;
	box4.physicsObject->confineToScreen = true;
	box4.physicsObject->setVelocity(0, 0);
	box4.physicsObject->usePassThroughDetection = true;
	box4.physicsObject->name = "box4";
	box4.physicsObject->setOnCollide(&collided);

	box5.physicsObject->useGravity = true;
	box5.physicsObject->useCollisions = true;
	box5.physicsObject->confineToScreen = true;
	box5.physicsObject->setVelocity(0, 0);
	box5.physicsObject->usePassThroughDetection = true;
	box5.physicsObject->name = "box5";
	box5.physicsObject->setOnCollide(&collided);

	//Add the boxes to the physicsEngine
	physics->registerObject(box1.physicsObject);
	physics->registerObject(box2.physicsObject);
	physics->registerObject(box3.physicsObject);
	physics->registerObject(box4.physicsObject);
	physics->registerObject(box5.physicsObject);

}



MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	//int speed = 3;
	if (eventSystem->isPressed(Key::SPACE)) {
		//Disable gravity for all objects
		if (box1.physicsObject->useGravity) {
			box1.physicsObject->useGravity = false;
			box2.physicsObject->useGravity = false;
			box3.physicsObject->useGravity = false;
			box4.physicsObject->useGravity = false;
			box5.physicsObject->useGravity = false;
			gravityStr = "Gravity: Off";
		} else {//Enable gravity
			box1.physicsObject->useGravity = true;
			box2.physicsObject->useGravity = true;
			box3.physicsObject->useGravity = true;
			box4.physicsObject->useGravity = true;
			box5.physicsObject->useGravity = true;
			gravityStr = "Gravity: On";
		}
		
	}
	else if (eventSystem->isPressed(Key::A)) {
		//Disable collisions
		if (box1.physicsObject->useCollisions) {
			box1.physicsObject->useCollisions = false;
			box2.physicsObject->useCollisions = false;
			box3.physicsObject->useCollisions = false;
			box4.physicsObject->useCollisions = false;
			box5.physicsObject->useCollisions = false;
			collisionStr = "Collision: Off";
		} else {//Enable collisions
			box1.physicsObject->useCollisions = true;
			box2.physicsObject->useCollisions = true;
			box3.physicsObject->useCollisions = true;
			box4.physicsObject->useCollisions = true;
			box5.physicsObject->useCollisions = true;
			collisionStr = "Collision: On";
		}
	}
}



void MyGame::update() {
	//box.x += velocity.x;
	//box.y += velocity.y;

	//Update all the boxes
	box1.update();
	box2.update();
	box3.update();
	box4.update();
	box5.update();


	//std::cout << "MyGame update: " << box.x << " " << box.y << "\n";

}


void MyGame::render() {
	//Draw all the boxes
	gfx->setDrawColor(SDL_COLOR_YELLOW);
	gfx->drawRect(box1);
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawRect(box2);
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(box3);
	gfx->setDrawColor(SDL_COLOR_WHITE);
	gfx->drawRect(box4);
	gfx->setDrawColor(SDL_COLOR_PINK);
	gfx->drawRect(box5);
	

	
}

void MyGame::renderUI() {
	//Draw text
	gfx->setDrawColor(SDL_COLOR_AQUA);
	gfx->drawText(gravityStr, 0, 50);
	gfx->drawText(collisionStr, 0, 200);

}