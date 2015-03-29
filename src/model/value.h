/**
 * \file value.h
 * \brief An abstract class who represent a values of a given type
 * \author Julia Bassoumi, Adrien Plazas
 * \version 0.2
 * \date 14/03/2015
 */

#pragma once

namespace stibbons {

/**
 * \class Type
 *
 * \brief The available value types.
 *
 * \author Julia Bassoumi
 */
enum class Type {
	NIL,
	NUMBER,
	BOOLEAN,
	STRING,
	COLOR,
	FUNCTION,
	TURTLE,
	ZONE,
	WORLD
};

/**
 * \class Value
 *
 * \brief An abstract class who represent a values of a given type.
 *
 * \author Julia Bassoumi
 */
class Value {
	public:
		/**
		 * Return the type of the value
		 * @return a Type
		 */
		virtual Type getType() const=0;
};

/**
 * \class GenericValue
 *
 * \brief An template class easing the implementation of a Value class.
 *
 * \author Adrien Plazas
 */
template<Type T>
class GenericValue : public virtual Value {
	public:
		virtual Type getType() const {
			return T;
		}
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
