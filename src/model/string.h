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
 * \file string.h
 * \brief A class representing a string
 * \author Julia Bassoumi, Adrien Plazas
 * \version 1.1
 * \date 14/03/2015
 */

#pragma once

#include "value.h"
#include "simple-value.h"

#include <string>

#include <memory>

namespace stibbons {

/**
 * \class String
 *
 * \brief A class representing a string.
 *
 * \author Julia Bassoumi, Adrien Plazas
 */
class String : public GenericValue<Type::STRING>, public SimpleValue<std::string> {
	public:
		using SimpleValue<std::string>::SimpleValue;
		virtual ~String () = default;

		virtual void reset ();

		/**
		 * Add a value to another
		 * \param other the other value
		 * \return the resulting value
		 */
		virtual ValuePtr add (ValuePtr other) throw(std::domain_error);

		/**
		 * Add a value to another
		 * \param other the other value
		 * \return the resulting value
		 */
		virtual StringPtr add (StringPtr other);

		/**
		 * Return whether the comparison value of two values
		 *
		 * Return a negative number if this value is lower than the other,
		 * return a positive number if this value is greater than the other,
		 * return 0 if they are equal.
		 *
		 * \param other the other value
		 * \return the comparison value of this and the other value
		 */
		virtual double compare (ValuePtr other);

		/**
		 * Return whether the comparison value of two values
		 *
		 * Return a negative number if this value is lower than the other,
		 * return a negative number if this value is greater than the other,
		 * return 0 if they are equal.
		 *
		 * \param other the other value
		 * \return the comparison value of this and the other value
		 */
		virtual double compare (StringPtr other);

		/**
		 * Return a string corresponding to the value
		 * \return a string corresponding to the value
		 */
		virtual string toString ();
};

typedef std::shared_ptr<String> StringPtr;

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
