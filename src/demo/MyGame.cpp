#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), box(100, 0, 30, 30), boxPo(new PhysicsObject(Point2(100, 100), 30, 30))
	, box2(0, 0, 30, 30), boxPo2(new PhysicsObject(Point2(110, 200), 30, 30)), box3(200, 0, 30, 30), boxPo3(new PhysicsObject(Point2(400, 200), 30, 30)) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);

	physics->windowSize = gfx->getCurrentWindowSize();

	gfx->useFont(font);
	gfx->setVerticalSync(true);

	boxPo->useGravity = true;
	boxPo->useCollisions = true;
	boxPo->confineToScreen = true;
	boxPo->setVelocity(getRandom(-10, 10), getRandom(-10, 10));

	boxPo2->useGravity = true;
	boxPo2->useCollisions = true;
	boxPo2->confineToScreen = true;
	boxPo2->setVelocity(getRandom(-10, 10), getRandom(-10, 10));

	boxPo3->useGravity = true;
	boxPo3->useCollisions = true;
	boxPo3->confineToScreen = true;
	boxPo3->setVelocity(getRandom(-10, 10), getRandom(-10, 10));



	physics->registerObject(boxPo);
	physics->registerObject(boxPo2);
	physics->registerObject(boxPo3);


    for (int i = 0; i < numKeys; i++) {
        std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
        k->isAlive = true;
        k->pos = Point2(getRandom(0, 750), getRandom(0, 550));
        gameKeys.push_back(k);
    }
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	//int speed = 3;

	//if (eventSystem->isPressed(Key::W)) {
	//	velocity.y = -speed;
	//}
}

void MyGame::update() {
	//box.x += velocity.x;
	//box.y += velocity.y;
	
	box.y = boxPo->getCenter().y;
	box.x = boxPo->getCenter().x;

	box2.y = boxPo2->getCenter().y;
	box2.x = boxPo2->getCenter().x;

	box3.y = boxPo3->getCenter().y;
	box3.x = boxPo3->getCenter().x;

	//std::cout << "MyGame update: " << box.x << " " << box.y << "\n";

	



	for (auto key : gameKeys) {
		if (key->isAlive && box.contains(key->pos)) {
			score += 200;
			key->isAlive = false;
			numKeys--;
		}
	}

	if (numKeys == 0) {
		gameWon = true;
	}
}

void MyGame::render() {
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(box);
	gfx->setDrawColor(SDL_COLOR_GREEN);
	gfx->drawRect(box2);
	gfx->setDrawColor(SDL_COLOR_YELLOW);
	gfx->drawRect(box3);


	gfx->setDrawColor(SDL_COLOR_YELLOW);
	for (auto key : gameKeys)
        if (key->isAlive)
		    gfx->drawCircle(key->pos, 5);
}

void MyGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	if (gameWon)
		gfx->drawText("YOU WON", 250, 500);
}