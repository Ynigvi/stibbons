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

#include "interpreter-exception.h"

namespace stibbons {
	InterpreterException::InterpreterException(const char* msg, yy::position pos): msg(msg), pos(pos) {}

	InterpreterException::InterpreterException(std::string msg, yy::position pos): msg(msg.c_str()), pos(pos) {}

	const char* InterpreterException::what() const throw() {
		std::ostringstream oss; 
		oss<<msg<<" at "<<pos.line<<":"<<pos.column<<".";
		return oss.str().c_str();
	}
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
