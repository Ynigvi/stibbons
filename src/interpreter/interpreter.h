/**
 * \file tree.h
 * \brief Interpreter class header.
 * \author Florian Galinier
 * \author Clément Simon
 * \version 0.3
 * \date 15/03/15
 *
 * Declaration of Interpreter class.
 *
 */

#pragma once

#include "y.tab.h"
#include "tree.h"
#include "../model/turtle.h"
#include "../model/table.h"
#include "../model/number.h"
#include "../model/string.h"
#include "../model/boolean.h"
#include "../model/color.h"
#include "../model/nil.h"

namespace stibbons {
	/**
	 * \class Interpreter
	 * \brief Class that will interpret stibbons language.
	 *
	 * This class will parse the standart input, generate a syntaxic tree and interpret it.
	 *
	 * \author Florian Galinier
	 * \author Clément Simon
	 */
	class Interpreter {
	public:
		/**
		 * Create a new interpreter
		 */
		Interpreter() = default;

		/**
		 * Interpret the tree and apply it to the turtle.
		 * \param turtle The turtle to run the program on.
		 * \param tree The syntaxic tree to interpret.
		 * \param hashTable A hashtable which contain parameters
		 * \return An int equal to 0 if no error has occurred.
		 */
		virtual Value* interpret(Turtle* turtle,
		                         const Tree*,
		                         Table* hashTable=nullptr) const;
		virtual Value* interpret(Agent* agent,
		                         const Tree*,
		                         Table* hashTable=nullptr) const;

	private:
		/**
		 * Create a function from a tree with a
		 * FCT or AGT node.
		 * \param tree A tree with the FCT or AGT root node. 
		 * \return The function corresponding to the tree
		 */
		virtual Function* getFunctionFromTree(const Tree*) const;

		/**
		 * Interpret a function (or function of a breed)
		 * \param fct A function that will be interpreted
		 * \param turtle The turtle to run the program on
		 * \param tree The Tree corresponding to function call
		 * \param hashTable The hashtable that will contains parameters of the function
		 * \param id The name of function for error message
		 * \return The Value returned by the Function fct
		 */
		virtual	Value* interpretFunction(Function*,
		                                 Turtle*,
										 const Tree*,
										 Table*, 
										 std::string) const;

		static inline yy::position getPosition(const Tree* tree);
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
