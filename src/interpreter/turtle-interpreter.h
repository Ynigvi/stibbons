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

#include "interpreter.h"
#include "../model/turtle.h"

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
  class TurtleInterpreter : public virtual Interpreter {
  public:

    /**
     * Create a new interpreter for a turtle
     */
	  TurtleInterpreter() = default;

    /**
     * Check if the agent is a world and call the interpret for a turtle
     * \param agent The agent to run the program on.
     * \param tree The syntaxic tree to interpret.
     * \param hashTable A hashtable which contain parameters
     * \return An int equal to 0 if no error has occurred.
     */
    virtual ValuePtr interpret(InterpreterManager& manager,
				AgentPtr agent,
			     const TreePtr,
			     TablePtr hashTable);


    /**
     * Interpret the tree and apply it to the turtle.
     * \param agent The agent to run the program on.
     * \param tree The syntaxic tree to interpret.
     * \param hashTable A hashtable which contain parameters
     * \return An int equal to 0 if no error has occurred.
     */
    virtual ValuePtr interpret(InterpreterManager& manager,
				TurtlePtr agent,
			     const TreePtr,
			     TablePtr hashTable);

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
