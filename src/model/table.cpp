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


#include "table.h"

#include "boolean.h"
#include "color.h"
#include "nil.h"
#include "number.h"
#include "string.h"

#include <sstream>

using namespace std;

namespace stibbons {

	void Table::setValue (pair<string, ValuePtr> pair) {
		lock_guard<recursive_mutex> lock(value_m);

		auto search = namedValues.find (pair.first);
		if ( search == namedValues.end())
			namedValues.insert(pair);
		else {
			//tryDelete (search->second);
			namedValues.erase(pair.first);
			namedValues.insert(pair);
		}
	}

	void Table::setValue (string key, ValuePtr value) {
		setValue (pair<string, ValuePtr>(key, value));
	}

	ValuePtr Table::getValue(string key) {
		lock_guard<recursive_mutex> lock(value_m);

		auto got = namedValues.find(key);

		if (got == namedValues.end())
			return Nil::getInstance();

		return got->second;
	}	

	bool Table::exists(string key) {
		lock_guard<recursive_mutex> lock(value_m);

		auto got = namedValues.find(key);

		return !(got == namedValues.end());

	}

	const unordered_map<string, ValuePtr>& Table::getNamedValues() {
		lock_guard<recursive_mutex> lock(value_m);

		return namedValues;
	}

	const map<long, ValuePtr>& Table::getIndexedValues() {
		lock_guard<recursive_mutex> lock(value_m);

		return indexedValues;
	}

	void Table::setValue (pair<long, ValuePtr> pair) {
		lock_guard<recursive_mutex> lock(value_m);

		auto search = indexedValues.find (pair.first);
		if ( search == indexedValues.end())
			indexedValues.insert(pair);
		else {
			//tryDelete (search->second);
			indexedValues.erase(pair.first);
			indexedValues.insert(pair);
		}
	}

	void Table::setValue (long key, ValuePtr value) {
		setValue (pair<long, ValuePtr>(key, value));
	}

	void Table::append (ValuePtr value) {
		lock_guard<recursive_mutex> lock(value_m);

		long key = indexedValues.empty() ? 0 :
			indexedValues.rbegin()->first + 1;

		indexedValues.insert(pair<long, ValuePtr>(key, value));
	}

	ValuePtr Table::getValue(long key) {
		lock_guard<recursive_mutex> lock(value_m);

		auto got = indexedValues.find(key);

		if (got == indexedValues.end())
			return Nil::getInstance();

		return got->second;
	}

	double Table::compare (ValuePtr other) {
		if (getType() != other->getType())
			return Value::compare (other);

		return compare (dynamic_pointer_cast<Table>(other));
	}

	double Table::compare(TablePtr other) {
		return this - other.get() ;
	}

	string Table::toString() {
		lock_guard<recursive_mutex> lock(value_m);

		std::ostringstream oss;

		oss << "{\n";

		for (auto value : indexedValues)
			oss << "\t" << value.first << ": " << value.second->toString() << "\n";

		for (auto value : namedValues)
			oss << "\t" << value.first << ": " << value.second->toString() << "\n";

		oss << "}\n";

		return oss.str();
	}

	int Table::length() {
		return indexedValues.size();
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
