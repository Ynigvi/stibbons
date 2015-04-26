/**
  * \file tree.h
  * \brief WorldInterpreter class header.
  * \author Florian Galinier
  * \author Clément Simon
  * \author Adrien Plazas
  * \version 0.4
  * \date 08/04/15
  *
  * Declaration of WorldInterpreter class.
  *
  */

#pragma once

#include "interpreter.h"

#include "tree.h"
#include "syntax-exception.h"
#include "../model/world.h"

namespace stibbons {

	/**
	 * \class WorldInterpreter
	 * \brief Class that will interpret stibbons language.
	 *
	 * This class will parse the standart input, generate a syntaxic tree and interpret it.
	 *
	 * \author Florian Galinier
	 * \author Clément Simon
	 */
	class WorldInterpreter : public virtual Interpreter {
	public:
		/**
		 * Create a new interpreter
		 * Parse a Stibbons program, store its syntactic tree and return
		 * the Wolrd generated by this tree.
		 *
		 * \param program The Stibbons program to parse.
		 */
		WorldInterpreter(std::string program) throw(SyntaxException);

		/**
		 * Get the world.
		 *
		 * \return The world associated to given program.
		 */
		virtual WorldPtr getWorld();

		/**
		 * Run the interpreter with the parsed program.
		 * \param world The world to run the program on.
		 */
		virtual void run();

	private:
		WorldPtr world;
		TreePtr tree;
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
