#pragma once

#include "changeable.h"
#include "color.h"
#include "point.h"
#include "world.h"
#include "line.h"
#include "agent.h"
#include "breed.h"
#include "value.h"
#include <future>
#include <cmath>
#include <stdexcept>
#include <system_error>
#include <unordered_map>

using namespace std;

namespace stibbons {

class World;
class Breed;

typedef unsigned long turtle_id;

/**
 *\class Turtle
 *
 *\brief class Turtle stibbons
 *
 *\author Julia Bassoumi
 */
class Turtle : public Point, public Changeable, public Agent{
	friend Breed;

	public:
		/**
		 * Create a turtle
		 */
		Turtle ();

		/**
		 * Create a turtle
		 *
		 */
		Turtle (Agent *parent, turtle_id id = 0);

		virtual Type getType() const;

		/**
		 * Set the value of angle
		 * @param new_var the new value of angle
		 */
		void setAngle (double new_var);

		/**
		 * Get the value of angle
		 * @return the value of angle
		 */
		double getAngle ();

		/**
		 * Set the value of id
		 * @param new_var the new value of id
		 */
		void setId (turtle_id new_var);

		/**
		 * Get the value of id
		 * @return the value of id
		 */
		turtle_id getId() const;

		/**
		 * Get the value of world
		 * @return the value of world
		 */
		World* getWorld ();

		/**
		 * Set the value for an axis
		 * @param axis the axis
		 * @param value the value
		 */
		void setColor (Color color);

		/**
		 * Get the value for an axis
		 * @return a reference to the value for an axis
		 */
		Color& getColor ();

		/**
		 * Get the value for an axis
		 * @return a constant reference to the value for an axis
		 */
		const Color& getColor () const;

		/**
		 * Go forward
		 * @param the distance to go
		 */
		void forward(double dist);

		/**
		 * Turn right at dist degree
		 * @param the degree to turn
		 */
		void turnRight(double angle);

		/**
		 * Turn left at dist degree
		 * @param the degree to turn
		 */
		void turnLeft(double angle);
		/**

		 * Let a trace on the map behind the turtle
		 */
		void penDown() throw (future_error);

		/**
		 * Stop to trace on the map behind the turtle
		 */
		void penUp() throw (future_error);

		/**
		 * Create a child turtle
		 *
		 * Create a new turtle whose parent is this turtle, and whose
		 * breed and color are this turtle's breed and color.
		 *
		 * @return the new child turtle
		 */
		Turtle *createChild();

	private:
	public:
		/**
		 * Special constructor for Breed
		 */
		Turtle (Breed *breed);

		/**
		 * Special constructor for Breed
		 */
		Turtle (Turtle *parent);

		turtle_id id;

		Breed *breed;

		double angle;
		Color color;
		Line* line;
		unordered_map<string,Value*> *properties;
	};
}

/*
 * Editor modelines  -  http://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: t
 * truncate-lines: 1
 * End:
 *
 * vim:set ft=cpp ts=4 sw=4 sts=4
 */
