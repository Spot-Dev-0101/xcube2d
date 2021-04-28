#include "PhysicsEngine.h"
#include <iostream>


PhysicsObject::PhysicsObject(const Point2 & center, float x, float y)
: center(center), lX(x), lY(y), hlX(x / 2.0f), hlY(y / 2.0f), force(0.0f, 0.0f) {}

bool PhysicsObject::isColliding(PhysicsObject & other) {
    Rectf r1 = { center.x - hlX, center.y - hlY, lX, lY };
    Rectf r2 = { other.center.x - other.hlX, other.center.y - other.hlY, other.lX, other.lY };

	return r1.intersects(r2);
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

void PhysicsObject::invertVelocityX(float decay) {
	velocity.x = -velocity.x *decay;
}

void PhysicsObject::invertVelocityY(float decay) {
	velocity.y = -velocity.y *decay;
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



		if (po->confineToScreen) {
			if (po->getCenter().x <= 0) {
				po->invertVelocityX(0.9f);
			}
			else if (po->getCenter().x >= windowSize.w - po->getLengthX()) {
				po->invertVelocityX(0.9f);
			}

			if (po->getCenter().y <= 0) {
				po->invertVelocityY(0.9f);
			}
			else if (po->getCenter().y >= windowSize.h - po->getLengthY()) {
				po->invertVelocityY(0.9f);
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

		//Handle collisions
		if (po->useCollisions) {
			for (std::shared_ptr<PhysicsObject> otherPo : objects) {
				if (po != otherPo && otherPo->useCollisions) {
					if (po->isColliding(*otherPo)) {
						//po->setVelocity(0, 0);
						//otherOP->setVelocity(0, 0);
						
						
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

						//std::cout << " Speed: " << speed << "\n";

						po->velocity.x -= (speed * collisionNormX);
						po->velocity.y -= (speed * collisionNormY);

						otherPo->velocity.x += (speed * collisionNormX);
						otherPo->velocity.y += (speed * collisionNormY);

						

					}
				}
			}
		}

		

		

		po->applyVelocity();
		//std::cout << "Physics Update " << po->center.x << " " << po->center.y << "\n";
	}
}