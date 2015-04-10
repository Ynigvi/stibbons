/**
 * \file standard-function.h
 * \brief Classes implementing standard functions
 * \author Adrien Plazas
 * \version 0.3
 * \date 10/04/2015
 */

#pragma once

#include "function.h"

namespace stibbons {

/**
 * class RandFunction
 *
 *\brief A class returning a random number.
 *
 *\author Adrien Plazas
**/
class RandFunction : public Function {
	public:
		virtual Value* exec (Agent* agent, Table* params);
};

/**
 * class PrintFunction
 *
 *\brief A class printing on the standard output.
 *
 *\author Adrien Plazas
**/
class PrintFunction : public Function {
	public:
		PrintFunction ();
		virtual Value* exec (Agent* agent, Table* params);
};

/**
 * class PrintlnFunction
 *
 *\brief A class printing a new line on the standard output.
 *
 *\author Adrien Plazas
**/
class PrintlnFunction : public Function {
	public:
		PrintlnFunction ();
		virtual Value* exec (Agent* agent, Table* params);
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
