#ifndef __PHYSICS_ENGINE_H__
#define __PHYSICS_ENGINE_H__

#include <vector>
#include <memory>

#include "GameMath.h"

static const float DEFAULT_GRAVITY = 0.25f;//default was 1.0f

class PhysicsObject;

class PhysicsEngine {
	friend class XCube2Engine;
	friend class PhysicsObject;
	private:
		Vector2f gravity;
		PhysicsEngine();

		std::vector<std::shared_ptr<PhysicsObject>> objects;

	public:
		/**
		* Note that gravity is naturally a negative value
		* update interval in seconds
		*/
		void setGravity(float gravityValue, float worldUpdateInterval);
		void update();

		void registerObject(std::shared_ptr<PhysicsObject>&);

		Dimension2i windowSize;
};

class PhysicsObject {
	friend class PhysicsEngine;
	protected:
		Point2 center;
		float lX, lY, hlX, hlY;	// lengths and half lengths

		Vector2f force;

		void applyForce(Vector2f &);
	public:
		PhysicsObject(const Point2 & center, float x, float y);

		bool useGravity = false;
		bool useAntiGravity = false;
		bool useCollisions = false;
		bool canMove = true;
		bool confineToScreen = false;

		Vector2f velocity;

		Point2 getCenter() { return center; }
		float getLengthX() { return lX; }
		float getLengthY() { return lY; }
		float getHalfLengthX() { return hlX; }
		float getHalfLengthY() { return hlY; }

		bool isColliding(PhysicsObject & other);

		float getDistanceTo(PhysicsObject& other);

		virtual void applyVelocity();
		virtual void setVelocity(float x, float y);
		virtual void invertVelocityX(float decay);
		virtual void invertVelocityY(float decay);

		

		/**
		* If we have different implementations of engines/gravity
		* this can be very useful
		*/
		virtual void applyGravity(Vector2f& gravity);
		virtual void applyAntiGravity(Vector2f& gravity);
};

#endif