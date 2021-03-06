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

#include "number.h"

#include <cmath>
#include <sstream>

using namespace std;

namespace stibbons {

void Number::reset () {
	value = 0.0;
}

ValuePtr Number::add (ValuePtr other) throw(std::domain_error) {
	switch (other->getType()) {
		case Type::NUMBER:
			return add (dynamic_pointer_cast<Number>(other));
		default:
			return Value::add(other);
	}
}

NumberPtr Number::add (NumberPtr other) {
	return make_shared<Number>(getValue() + other->getValue());
}

ValuePtr Number::substract (ValuePtr other) throw(std::domain_error) {
	switch (other->getType()) {
		case Type::NUMBER:
			return substract (dynamic_pointer_cast<Number>(other));
		default:
			return Value::substract(other);
	}
}

NumberPtr Number::substract (NumberPtr other) {
	return make_shared<Number>(getValue() - other->getValue());
}

ValuePtr Number::multiply (ValuePtr other) throw(std::domain_error) {
	switch (other->getType()) {
		case Type::NUMBER:
			return multiply (dynamic_pointer_cast<Number>(other));
		default:
			return Value::multiply(other);
	}
}

NumberPtr Number::multiply (NumberPtr other) {
	return make_shared<Number>(getValue() * other->getValue());
}

ValuePtr Number::divide (ValuePtr other) throw(std::domain_error) {
	switch (other->getType()) {
		case Type::NUMBER:
			return divide (dynamic_pointer_cast<Number>(other));
		default:
			return Value::divide(other);
	}
}

NumberPtr Number::divide (NumberPtr other) throw(std::domain_error) {
	auto number = other->getValue();
	if (number == 0.0)
		throw std::domain_error("Can't divide by 0");

	return make_shared<Number>(getValue() / other->getValue());
}

ValuePtr Number::modulo (ValuePtr other) throw(std::domain_error) {
	switch (other->getType()) {
		case Type::NUMBER:
			return modulo (dynamic_pointer_cast<Number>(other));
		default:
			return Value::modulo(other);
	}
}

NumberPtr Number::modulo (NumberPtr other) throw(std::domain_error) {
	auto number = other->getValue();
	if (number == 0.0)
		throw std::domain_error("Can't divide by 0");

	return make_shared<Number>(fmod(getValue(), other->getValue()));
}

NumberPtr Number::unaryMinus () throw(std::domain_error){
	auto number = this->getValue();
	auto val = - number;
	return make_shared<Number>(val);
}


double Number::compare (ValuePtr other) {
	if (getType() != other->getType())
		return Value::compare (other);

	return compare (dynamic_pointer_cast<Number>(other));
}

double Number::compare(NumberPtr other) {;
	return getValue () - other->getValue ();
}

string Number::toString () {
	std::ostringstream oss;

	oss << getValue();

	return oss.str();
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
