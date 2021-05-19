#include "PhysicsEngine.h"
#include <iostream>


PhysicsObject::PhysicsObject(const Point2 & center, float x, float y)
: center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.0f, 0.0f) {}

bool PhysicsObject::isColliding(PhysicsObject & other) {
    //Rectf r1 = { center.x - hlX, center.y - hlY, lX, lY };
    //Rectf r2 = { other.center.x - other.hlX, other.center.y - other.hlY, other.lX, other.lY };

	//return r1.intersects(r2);
	if (usePassThroughDetection == false) {
		//basic intersection detection
		if (other.center.x > lX + center.x || center.x > other.lX + other.center.x || other.center.y > lY + center.y || center.y > other.lY + other.center.y) {
			return false;
		}
		return true;
	}
	else {
		//bool result = true;
		//The distance between each point it checks
		//Example would be a distance of 2 if there are 5 points and the velocity is 10
		float distanceX = (velocity.x / amountOfPassThroughPoints);
		float distanceY = (velocity.y / amountOfPassThroughPoints);
		float otherDistanceX = (other.velocity.x / amountOfPassThroughPoints);
		float otherDistanceY = (other.velocity.y / amountOfPassThroughPoints);
		//check I points between the current and next frame
		for (size_t i = 1; i < amountOfPassThroughPoints+1; i++)
		{
			if ((other.center.x + otherDistanceX*i) > lX + (center.x + distanceX * i)
				|| (center.x + distanceX * i) > other.lX + (other.center.x + otherDistanceX * i)
				|| (other.center.y + otherDistanceY * i) > lY + (center.y + distanceY * i)
				|| (center.y + distanceY * i) > other.lY + (other.center.y + otherDistanceY * i)) {
				return false;
				//result = false;
			}
		}
		//return result;
		return true;
	}
	
	
}

void PhysicsObject::applyForce(Vector2f & v) {
	//velocity += v;
	velocity.x += v.x;
	velocity.y += v.y;
}

void PhysicsObject::applyGravity(Vector2f & gravity) {
	//velocity += gravity;
	velocity.x += gravity.x;
	velocity.y += gravity.y;
}

void PhysicsObject::applyAntiGravity(Vector2f& gravity) {
	velocity.x -= gravity.x;
	velocity.y -= gravity.y;
}

void PhysicsObject::setVelocity(float x, float y) {
	velocity.x = x;
	velocity.y = y;
}

void PhysicsObject::applyVelocity() {
	center += velocity;
}

float PhysicsObject::getDistanceTo(PhysicsObject& other) {
	//std::cout << ((float)(other.center.x - center.x) * (float)(other.center.x - center.x) + (float)(other.center.y - center.y) * (float)(other.center.y - center.y));
	return sqrt(((float)(other.center.x - center.x) * (float)(other.center.x - center.x) + (float)(other.center.y - center.y) * (float)(other.center.y - center.y)));
}

void PhysicsObject::invertVelocityX(Vector2f& gravity) {
	velocity.x = (-velocity.x);// - gravity.x;
}

void PhysicsObject::invertVelocityY(Vector2f& gravity) {
	velocity.y = (-velocity.y);// - gravity.y;
}



/* RECT PHYSICS */
RectPhysics::RectPhysics(float x, float y, float w, float h) : Rect(x, y, w, h), physicsObject( new PhysicsObject(Point2(x, y), w, h)){}

void RectPhysics::update()
{
	//Set the position of the drawable box to the physics object
	x = physicsObject->getCenter().x;
	y = physicsObject->getCenter().y;
}

void PhysicsObject::setOnCollide(std::shared_ptr<void>& (*collisionFunction)(std::shared_ptr<PhysicsObject> self, std::shared_ptr<PhysicsObject> other))
{
	onCollide = collisionFunction;
	hasSetCollider = true;
}




/* PHYSICS ENGINE */

PhysicsEngine::PhysicsEngine() : gravity(Vector2f(0, DEFAULT_GRAVITY)) {}

void PhysicsEngine::setGravity(float val, float interval) {
	gravity = Vector2f(0, val * interval);
}

void PhysicsEngine::registerObject(std::shared_ptr<PhysicsObject> &obj) {
	objects.push_back(obj);
}

void PhysicsEngine::update() {
	//Loop through all the physics objects
	for (std::shared_ptr<PhysicsObject> po : objects) {


		//Handle collisions
		if (po->useCollisions) {
			for (std::shared_ptr<PhysicsObject> otherPo : objects) {
				//Only check collisions if the other is also using collisions
				if (po != otherPo && otherPo->useCollisions) {
					if (po->isColliding(*otherPo)) {

						//Get the collision vector
						float collisionVectorX = otherPo->center.x - po->center.x;
						float collisionVectorY = otherPo->center.y - po->center.y;

						float distance = po->getDistanceTo(*otherPo);
						//std::cout << " distance: " << distance;

						float collisionNormX = collisionVectorX / distance;
						float collisionNormY = collisionVectorY / distance;

						//std::cout << " collisionNormY: " << collisionNormY;

						float relVelocityX = po->velocity.x - otherPo->velocity.x;
						float relVelocityY = po->velocity.y - otherPo->velocity.y;

						//std::cout << " relVelocityY: " << relVelocityY;

						float speed = relVelocityX * collisionNormX + relVelocityY * collisionNormY;


						po->velocity.x -= (speed * collisionNormX);
						po->velocity.y -= (speed * collisionNormY);

						otherPo->velocity.x += (speed * collisionNormX);
						otherPo->velocity.y += (speed * collisionNormY);

						//run collision function if it exists
						if (po->hasSetCollider) {
							po->onCollide(po, otherPo);
						}
						

					}
				}
			}
		}

		if (po->confineToScreen) {
			//LEFT
			if (po->getCenter().x <= 0) {
				po->invertVelocityX(gravity);
				po->center.x = 0;
			}//RIGHT
			else if (po->getCenter().x >= windowSize.w - po->getLengthX()) {
				po->invertVelocityX(gravity);
				po->center.x = windowSize.w - po->getLengthX();
			}

			//TOP
			if (po->getCenter().y <= 0) {
				po->invertVelocityY(gravity);
				po->center.y = 0;
			}//BOTTOM
			else if (po->getCenter().y >= windowSize.h - po->getLengthY()) {
				po->invertVelocityY(gravity);
				//po->velocity.y = -abs(po->velocity.y) * 0.75f;
				//po->center.y = windowSize.h - po->getLengthY();
			}
			//std::cout << "" << po->velocity.x << " " << po->velocity.y << "\n";
		}

		//Apply gravity
		if (po->useGravity) {
			po->applyGravity(gravity);
		}
		//Apply anti gravity
		if (po->useAntiGravity) {
			po->applyAntiGravity(gravity);
		}
		
		
		

		po->applyVelocity();
		//std::cout << "Physics Update " << po->center.x << " " << po->center.y << "\n";

		
	}
}