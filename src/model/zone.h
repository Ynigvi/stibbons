/*
 * This file is part of Stibbons.

 * Stibbons is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Stibbons is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with Stibbons.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file zone.h
 * \brief The Zone header class
 * \author Julia Bassoumi, Adrien Plazas
 * \version 1.1
 * \date 14/03/2015
 */
#pragma once

#include "world.h"
#include "color.h"
#include "number.h"
#include "string.h"
#include "boolean.h"
#include "type.h"
#include "agent.h"
#include <stdexcept>
#include <system_error>
#include <unordered_map>
#include <mutex>
#include "json_spirit.h"
#include "json_spirit_writer_template.h"

using namespace json_spirit;


namespace stibbons {

class World;
typedef unsigned long zone_id;

/**
 * \class Zone
 *
 * \brief class Zone
 *
 * \author Julia Bassoumi
 */
class Zone : public Agent{
	public:
		/**
		 * Create a zone
		 * \param parent the parent of the current zone
		 */
		static ZonePtr construct (AgentPtr parent);

		/**
		 * Create a copy of a zone
		 * \param other the other zone
		 */
		Zone (Zone &other);

		/**
		 * Move a zone
		 * \param other the other zone
		 */
		Zone (Zone &&other);

		/**
		 * Copy of a zone
		 * \param other the other zone
		 */
		Zone& operator= (Zone& other);

		/**
		 * Move a zone
		 * \param other the other zone
		 */
		Zone& operator= (Zone&& other);

		virtual Type getType() const;

		/**
		 * Add a property
		 * \param key the key of the property
		 * \param value the value of the property
		 */
		virtual void setProperty (string key, ValuePtr value);

		 /**
		 * Get the value of the propertie p
		 * \return the value of propertie p
		 */
		virtual ValuePtr getProperty(string p);

		/**
		 * Set the value for an axis
		 * \param axis the axis
		 * \param value the value
		 */
		void setColor (Color color);

		/**
		 * Get the value for an axis
		 * \return a reference to the value for an axis
		 */
		Color getColor ();

		/**
		 * Get the value of world
		 * \return the value of world
		 */
		WorldPtr getWorld ();

		/**
		 * Set the value of id
		 * \param the id
		 */
		void setId (zone_id z);

		/**
		 * Get the value of id
		 * \return the value of id
		 */
		zone_id getId ();

		void changed ();

		/**
		 * Return a string corresponding to the value
		 * \return a string corresponding to the value
		 */
		virtual string toString ();

		/**
		 * Return an object which contain the zone's properties
		 * \return an json Object
		 */
		Object exportZone();

	protected:
		/**
		 * Create a zone
		 * \param parent the parent of the current zone
		 */
		Zone (AgentPtr parent);

		/**
		 * Initialize the zone
		 */
		void init ();

	private:
		Color color;

		zone_id id;
		recursive_mutex value_m;
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
