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

#include "interpreter.h"

#include <cstdio>
#include <cstring>

#include "../model/user-function.h"
#include "turtle-interpreter.h"
#include "world-interpreter.h"
#include "y.tab.h"

namespace stibbons {

	yy::position Interpreter::getPosition(const TreePtr tree) {
		return yy::position(nullptr,
		                    std::get<0>(tree->getPosition()),
		                    std::get<0>(tree->getPosition()));
	}	

	ValuePtr Interpreter::interpret(InterpreterManager& manager,
									AgentPtr agent,
									const TreePtr tree,
									TablePtr hashTable) {
		inPauseFlag = false;
		manager.checkExit();
		manager.checkHalt();
		manager.wait();

		if(tree != nullptr) {
			switch(std::get<0>(tree->getNode())) {
				//Sequence case:
			case 0: {
				ValuePtr res = Nil::getInstance();
				if(!tree->isLeaf()) {
					auto children = tree->getChildren();
					for(auto child : children) res = interpret(manager,agent,child,hashTable);
					return res;
				}
			}
				//Loop cases:
			case yy::parser::token::WHL: {
				auto val = this->interpret(manager,agent,tree->getChild(0),hashTable);
				ValuePtr res = Nil::getInstance();;
				if(val->getType() != Type::BOOLEAN) 
					throw SemanticException("WHILE",
					                        Type::BOOLEAN,
					                        val->getType(),
					                        getPosition(tree));
				while(dynamic_pointer_cast<Boolean>(val)->getValue()) {
					res = this->interpret(manager,agent,tree->getChild(1),hashTable);
					manager.checkHalt();
					manager.wait();
					val = this->interpret(manager,agent,tree->getChild(0));
					if(val->getType() != Type::BOOLEAN)
						throw SemanticException("WHILE",
												Type::BOOLEAN,
												val->getType(),
												getPosition(tree));
				}
				return res;
			}
			case yy::parser::token::RPT: {
				auto val = this->interpret(manager,agent,tree->getChild(0),hashTable);
				ValuePtr res;
				if(val->getType() != Type::NUMBER)
					throw SemanticException("REPEAT",
					                        Type::NUMBER,
					                        val->getType(),
					                        getPosition(tree));
				auto nb = dynamic_pointer_cast<Number>(val)->getValue();
				for(auto i=0;i<nb;i++) {
					res = this->interpret(manager,agent,tree->getChild(1),hashTable);
				}
				return res;
			}
			case yy::parser::token::FOR: {
				auto key_value = tree->getChild(0);
				auto key = (key_value->getChildren().size() > 1)?key_value->getChild(1):nullptr;
				std::string key_id;
				if(key)
					key_id = dynamic_pointer_cast<String>(std::get<1>(key->getNode()))->getValue();
				auto id = dynamic_pointer_cast<String>(std::get<1>(key_value->getNode()))->getValue();
				auto table = this->interpret(manager,agent,key_value->getChild(0),hashTable);
				ValuePtr res = Nil::getInstance();
				if(table->getType() != Type::TABLE)
					throw SemanticException("FOR",
					                        Type::TABLE,
					                        table->getType(),
					                        getPosition(tree));
				auto indexedValues = dynamic_pointer_cast<Table>(table)->getIndexedValues();
				auto namedValues = dynamic_pointer_cast<Table>(table)->getNamedValues();
				//update indexedValues in properties
				for(auto p : indexedValues) {
					if(key)
						agent->setProperty(key_id,make_shared<Number>(p.first));
					agent->setProperty(id,p.second);
					res = this->interpret(manager,agent,tree->getChild(1),hashTable);
				}
				//update namedValues in properties
				for(auto p : namedValues) {
					if(key)
						agent->setProperty(key_id,make_shared<String>(p.first));
					agent->setProperty(id,p.second);
					res = this->interpret(manager,agent,tree->getChild(1),hashTable);
				}
				return res;
			}
				//Conditionnal cases:
			case yy::parser::token::IF: {
				auto cond = this->interpret(manager,agent,tree->getChild(0),hashTable);
				if(cond->getType() != Type::BOOLEAN) 
					throw SemanticException("IF",
					                        Type::BOOLEAN,
					                        cond->getType(),
					                        getPosition(tree));
				if(dynamic_pointer_cast<Boolean>(cond)->getValue())
					return this->interpret(manager,agent,tree->getChild(1),hashTable);
				if(tree->getChildren().size() > 2)
					return this->interpret(manager,agent,tree->getChild(2),hashTable);
				break;
			}
				//Variable cases:
			case yy::parser::token::ID: {
				auto id = dynamic_pointer_cast<String>(std::get<1>(tree->getNode()))->getValue();
				return (hashTable && hashTable->exists(id)) ?
					hashTable->getValue(id) :
					agent->getProperty(id);
			}
			case yy::parser::token::TAB_ID: {
				auto tab = this->interpret(manager,agent,tree->getChild(0),hashTable);
				if(tab->getType() != Type::TABLE) 
					throw SemanticException("[]",
					                        Type::TABLE,
					                        tab->getType(),
					                        getPosition(tree));
				//Get correspondante value with key or position in table
				auto key = this->interpret(manager,agent,tree->getChild(1),hashTable);
				if(key->getType() == Type::STRING) {
					return dynamic_pointer_cast<Table>(tab)
						->getValue(dynamic_pointer_cast<String>(key)->getValue());
				}
				else if (key->getType() == Type::NUMBER) {
					return dynamic_pointer_cast<Table>(tab)
						->getValue(dynamic_pointer_cast<Number>(key)->getValue());
				}
				else {
					throw SemanticException("TABLE KEY",
											Type::STRING,
											Type::NUMBER,
											key->getType(),
											getPosition(tree));
				}
			}
			case '=':
				return affectationOp(manager,agent,tree,hashTable);
				//Attribute cases:
			case yy::parser::token::ATT_ID: {
				//get the value attribute of corresponding id
				auto t = this->interpret(manager,agent,tree->getChild(0),hashTable);	
				AgentPtr target;
				if(t->getType() != Type::TURTLE 
				   && t->getType() != Type::WORLD 
				   && t->getType() != Type::ZONE)
					throw SemanticException(".",
											Type::AGENT,
											t->getType(),
											getPosition(tree));
				target = dynamic_pointer_cast<Agent>(t);
				auto id = dynamic_pointer_cast<String>(std::get<1>(tree->getNode()))->getValue();
				return target->getProperty(id);
			}
				//Type cases:
			case yy::parser::token::NUMBER:
			case yy::parser::token::STRING:
			case yy::parser::token::COLOR:
			case yy::parser::token::BOOLEAN:
			case yy::parser::token::NIL:
			case yy::parser::token::TYPE:
				return std::get<1>(tree->getNode());
			case yy::parser::token::TABLE:
				{
					//create a table in current agent
					auto val = make_shared<Table>();
					auto children = tree->getChildren();
					if(children.size() > 0) {
						if(std::get<0>(children.at(0)->getNode()) == yy::parser::token::PAIR) {
							for(auto s : children) {
								auto key = this->interpret(manager,agent,s->getChild(0),hashTable);
								auto value = this->interpret(manager,agent,s->getChild(1),hashTable);
								if(key->getType() == Type::STRING) {
									val->setValue(
										dynamic_pointer_cast<String>(key)->getValue(),
										value
									);
								}
								else if (key->getType() == Type::NUMBER) {
									val->setValue(
										dynamic_pointer_cast<Number>(key)->getValue(),
										value
									);
								}
								else {
									throw SemanticException("TABLE KEY",
															Type::STRING,
															Type::NUMBER,
															key->getType(),
															getPosition(tree));
								}
							}
						}
						else {
							for(auto s : children) {
								val->append(this->interpret(manager,agent,s,hashTable));
							}
						}
					}
					return val;
				}
				//Arithmetic cases:
			case '+':
				try {
					auto val1 = this->interpret(manager,agent,tree->getChild(0), hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1), hashTable);
					return val1->add(val2);
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			case '-':
				try {
					auto val1 = this->interpret(manager,agent,tree->getChild(0), hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1), hashTable);
					return val1->substract(val2);
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			case yy::parser::token::UNARYMINUS: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				if(val1->getType() != Type::NUMBER) 
					throw SemanticException("-",
					                        Type::NUMBER,
					                        val1->getType(),
					                        getPosition(tree));
				return dynamic_pointer_cast<Number>(val1)->unaryMinus();
			}
			case '*':
				try {
					auto val1 = this->interpret(manager,agent,tree->getChild(0), hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1), hashTable);
					return val1->multiply(val2);
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			case '/':
				try {
					auto val1 = this->interpret(manager,agent,tree->getChild(0), hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1), hashTable);
					return val1->divide(val2);
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			case '%':
				try {
					auto val1 = this->interpret(manager,agent,tree->getChild(0), hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1), hashTable);
					return val1->modulo(val2);
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
				//Boolean operation cases:
			case yy::parser::token::AND: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
				if(val1->getType() != Type::BOOLEAN || val2->getType() != Type::BOOLEAN) 
					throw SemanticException("&",
					                        Type::BOOLEAN, Type::BOOLEAN,
					                        val1->getType(), val2->getType(),
					                        getPosition(tree));
				return make_shared<Boolean>((dynamic_pointer_cast<Boolean>(val1)->getValue()) && (dynamic_pointer_cast<Boolean>(val2)->getValue()));
			}
			case yy::parser::token::OR: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
				if(val1->getType() != Type::BOOLEAN || val2->getType() != Type::BOOLEAN) 
					throw SemanticException("|",
					                        Type::BOOLEAN, Type::BOOLEAN,
					                        val1->getType(), val2->getType(),
					                        getPosition(tree));
				return make_shared<Boolean>((dynamic_pointer_cast<Boolean>(val1)->getValue()) || (dynamic_pointer_cast<Boolean>(val2)->getValue()));
			}
			case yy::parser::token::XOR: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
				if(val1->getType() != Type::BOOLEAN || val2->getType() != Type::BOOLEAN) 
					throw SemanticException("^",
					                        Type::BOOLEAN, Type::BOOLEAN,
					                        val1->getType(), val2->getType(),
					                        getPosition(tree));
				return make_shared<Boolean>((dynamic_pointer_cast<Boolean>(val1)->getValue()) ^ (dynamic_pointer_cast<Boolean>(val2)->getValue()));
			}
			case yy::parser::token::NOT: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				if(val1->getType() != Type::BOOLEAN) 
					throw SemanticException("!",
					                        Type::BOOLEAN,
					                        val1->getType(),
					                        getPosition(tree));
				return make_shared<Boolean>(!(dynamic_pointer_cast<Boolean>(val1)->getValue()));
			}
			case yy::parser::token::EQ: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
				return make_shared<Boolean>(val1->isEqual(val2));
			}
			case yy::parser::token::NEQ: {
				auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
				auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
				return make_shared<Boolean>(val1->isDifferent(val2));
			}
			case yy::parser::token::GT: {
				try{
					auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
					if(val1->getType() != val2->getType())
						throw SemanticException(">",
												val1->getType(),val1->getType(),
												val1->getType(),val2->getType(),
												getPosition(tree));
					return make_shared<Boolean>(val1->isGreater(val2));
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			}
				break; 
			case yy::parser::token::GEQ: {
				try{
					auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
					if(val1->getType() != val2->getType())
						throw SemanticException(">=",
												val1->getType(),val1->getType(),
												val1->getType(),val2->getType(),
												getPosition(tree));
					return make_shared<Boolean>(val1->isGreaterOrEqual(val2));
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			} 
			case yy::parser::token::LS: {
				try{
					auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
					if(val1->getType() != val2->getType())
						throw SemanticException("<",
												val1->getType(),val1->getType(),
												val1->getType(),val2->getType(),
												getPosition(tree));
					return make_shared<Boolean>(val1->isLower(val2));
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			}
			case yy::parser::token::LEQ: {
				try{
					auto val1 = this->interpret(manager,agent,tree->getChild(0),hashTable);
					auto val2 = this->interpret(manager,agent,tree->getChild(1),hashTable);
					if(val1->getType() != val2->getType())
						throw SemanticException("<=",
												val1->getType(),val1->getType(),
												val1->getType(),val2->getType(),
												getPosition(tree));
					return make_shared<Boolean>(val1->isLowerOrEqual(val2));
				}
				catch (std::domain_error e) {
					throw SemanticException(e.what(), getPosition(tree));
				}
			}
				// New agent
			case yy::parser::token::NEW:
				//create new agent
				return newOp(manager,agent,tree,hashTable);
				// Functions
			case yy::parser::token::FCT: {
				//definition of new function
				auto id = dynamic_pointer_cast<String>(std::get<1>(tree->getNode()))->getValue();
				auto fct = this->getFunctionFromTree(manager,tree);
				agent->setProperty(id, fct);
			}
				break;
			case yy::parser::token::CALL:
				//call function
				return callOp(manager,agent,tree,hashTable);
			default :
				return nullptr;
			}
		}
		return Nil::getInstance();
	}

	FunctionPtr Interpreter::getFunctionFromTree(InterpreterManager& manager, const TreePtr tree) {
		auto fctTree = tree->getChild(0);
		auto params = std::vector<std::string>();
		auto children = tree->getChildren();
		for(size_t i=1;i<children.size();i++) {
			params.push_back(
				dynamic_pointer_cast<String>(
					std::get<1>(
						children.at(i)->getNode()
					)
				)->getValue()
			);
		}
		return make_shared<UserFunction>(manager,fctTree,params);
	}

	TablePtr Interpreter::getParams(InterpreterManager& manager,
	                                FunctionPtr fct,
	                                AgentPtr agent,
	                                const TreePtr tree,
	                                TablePtr hashTable,
	                                std::string id) {
		auto newHashTable = make_shared<Table>();

		if(tree) {
			if(fct->getParams().size() != tree->getChildren().size()) {
				std::ostringstream oss;
				oss<<"No matching function for "
				   <<id
				   <<" with "
				   <<tree->getChildren().size()
				   <<" parameters";
				throw SemanticException(oss.str().c_str(),
										getPosition(tree));
			}
			for(size_t i=0;i<fct->getParams().size();i++) {
				auto val = Interpreter::interpret(manager,agent,tree->getChild(i),hashTable);
				newHashTable->setValue(fct->getParams().at(i),val);
			}
		}

		return newHashTable;
	}

	ValuePtr Interpreter::interpretFunction(FunctionPtr fct,
	                                        AgentPtr agent,
	                                        TablePtr params) {
		return (*fct)(agent, params);
	}

	inline ValuePtr Interpreter::affectationOp(InterpreterManager& manager,AgentPtr agent,TreePtr tree, TablePtr hashTable){
		auto val = this->interpret(manager,agent,tree->getChild(1),hashTable);
		auto child = tree->getChild(0)->getNode();
		AgentPtr target;

		if (std::get<0>(child) == yy::parser::token::TAB_ID) {
			auto tab = this->interpret(manager,agent,tree->getChild(0)->getChild(0),hashTable);
			if(tab->getType() != Type::TABLE) 
				throw SemanticException("[]",
										Type::TABLE,
										tab->getType(),
										getPosition(tree));
			if(tree->getChild(0)->getChildren().size() > 1) {
				auto key = this->interpret(manager,agent,tree->getChild(0)->getChild(1),hashTable);
				if(key->getType() == Type::STRING) {
					dynamic_pointer_cast<Table>(tab)
						->setValue(dynamic_pointer_cast<String>(key)->getValue(),val);
				}
				else if (key->getType() == Type::NUMBER) {
					dynamic_pointer_cast<Table>(tab)
						->setValue(dynamic_pointer_cast<Number>(key)->getValue(),val);
				}
				else {
					throw SemanticException("TABLE KEY",
											Type::STRING,
											Type::NUMBER,
											key->getType(),
											getPosition(tree));
				}
			}
			else
				dynamic_pointer_cast<Table>(tab)->append(val);
		}
		else {
			auto id = dynamic_pointer_cast<String>(std::get<1>(child))->getValue();
			if (std::get<0>(child) == yy::parser::token::ATT_ID) {
				auto t = this->interpret(manager,agent,tree->getChild(0)->getChild(0),hashTable);
				if(t->getType() != Type::TURTLE 
				   && t->getType() != Type::WORLD 
				   && t->getType() != Type::ZONE)
					throw SemanticException(".",
											Type::TURTLE,
											t->getType(),
											getPosition(tree));
				target = dynamic_pointer_cast<Agent>(t);
			}
			else
				target = agent;
				
			if(hashTable) {
				auto got = hashTable->getValue(id);
				if (got->getType() != Type::NIL) {
					hashTable->setValue(id,val);
				}
			}
			target->setProperty(id,val);
		}				

		return val;
	}

	inline TablePtr Interpreter::newOp(InterpreterManager& manager,AgentPtr agent,TreePtr tree, TablePtr hashTable){
		auto type = std::get<1>(tree->getNode());
		std::string id;
		TreePtr paramTree;
		BreedPtr breed;
		auto turtleTable = make_shared<Table>();
		auto nb = this->interpret(manager,agent,tree->getChild(0),hashTable);

		if(nb->getType() != Type::NUMBER)
			throw SemanticException("NEW",
									Type::NUMBER,
									nb->getType(),
									getPosition(tree));					
				
		if(type == nullptr) {
			id = "anonym agent";
			auto function = make_shared<UserFunction>(manager,tree->getChild(1),vector<std::string>());
			breed = agent->getWorld()->createBreed(function);
			paramTree = nullptr;

		}
		else {
			id = dynamic_pointer_cast<String>(type)->getValue();
			breed = agent->getWorld()->getBreed(id);
			paramTree = tree->getChild(1);
		}

		for(auto i = 0;i<dynamic_pointer_cast<Number>(nb)->getValue();i++) {
			TurtlePtr newTurtle;
			auto fct = breed->getFunction();
			if(agent->getType() == Type::TURTLE)
				newTurtle = breed->createTurtle(dynamic_pointer_cast<Turtle>(agent));
			else
				newTurtle = breed->createTurtle(agent);					
			auto params = getParams(manager,fct,agent,paramTree,hashTable,id);

			manager.interpret_async<TurtleInterpreter>
				(fct, newTurtle, params);

			turtleTable->append(newTurtle);
		}
			
		return turtleTable;
	}

	inline ValuePtr Interpreter::callOp(InterpreterManager& manager,AgentPtr agent,TreePtr tree, TablePtr hashTable){
		auto id = dynamic_pointer_cast<String>(std::get<1>(tree->getNode()))->getValue();
		auto fctval = (hashTable && hashTable->exists(id)) ?
			hashTable->getValue(id) :
			agent->getProperty(id);
		auto fct = dynamic_pointer_cast<Function>(fctval);
		if(fct == nullptr)
			throw SemanticException("Try to eval a non function value",
									getPosition(tree));
		if(fct->getType() != Type::FUNCTION)
			throw SemanticException("()",
									Type::FUNCTION,
									fct->getType(),
									getPosition(tree));
		auto params = getParams(manager,fct,agent,tree,hashTable,id);
		try {
			return this->interpretFunction(fct,agent,params);
		}
		catch (std::domain_error e) {
			throw SemanticException(e.what(), getPosition(tree));
		}
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
