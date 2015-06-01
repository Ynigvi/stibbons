/**
 * \file tree.h
 * \brief Tree class header.
 * \author Florian Galinier
 * \author Clément Simon
 * \version 1.0
 * \date 13/03/15
 *
 * Declaration of Tree class (used by Interpreter class).
 *
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <tuple>
#include "../model/value.h"

namespace stibbons {

	class Tree;

	typedef std::shared_ptr<Tree> TreePtr;

	/**
	  * \class Tree
	  * \brief Class that will represent a syntaxic tree.
	  *
	  * This tree will be generated by syntaxic analyser and
	  * can be interpreted.
	  *
	  * \author Florian Galinier
	  * \author Clément Simon
	  */
	class Tree {
	private:
		std::pair<int,ValuePtr> node;
		std::vector<TreePtr > children;
		std::pair<int,int> position;
	public:
		/**
		 * Create a new tree from a token_type and a value
		 * \param token The kind of token that is used to create the node
		 * \param val The value (if needed) corresponding to the token
		 */
		Tree(int token = 0, ValuePtr val=nullptr);

		/**
		 * Destroy a tree and his children.
		 *
		 */
		virtual ~Tree() = default;

		/**
		 * Test if the node is a leaf.
		 * \return true is current tree has no children
		 */
		virtual bool isLeaf() const;

		/**
		 * Return current node.
		 * \return A pair of <token_type,Value> corresponding to current node
		 */
		virtual std::pair<int,ValuePtr> getNode() const;

		/**
		 * If current tree node is equal to t root node,
		 * append the children of t to the current node.
		 *
		 * \param t A Tree with root node equal to current Tree root node.
		 */ 
		void mergeTree(TreePtr );

		/**
		 * Append the children of t to the current node.
		 *
		 * \param t A Tree.
		 */ 
		void appendChildren(TreePtr );

		/**
		 * Create and append a child to current tree.
		 * \param token The kind of token that is used to create the child
		 * \param val The value (if needed) corresponding to the child's token
		 */
		virtual void addChild(int,ValuePtr);

		/**
		 * Append a tree child to current tree.
		 * \param t The tree to append
		 */
		virtual void addChild(TreePtr );

		/**
		 * Return all children of current tree
		 * \return a vector that contains all children of the Tree
		 */
		virtual const std::vector<TreePtr >& getChildren() const;

		/**
		 * Return a child identify by his position
		 * \param pos the position of the needed child
		 * \return the requested child (TreePtr  type)
		 */
		virtual TreePtr  getChild(size_t pos=0) const;

		/**
		 * Set the position of the token in file
		 * \param pos a pair <line,column>
		 */
		virtual void setPosition(std::pair<int,int>);

		/**
		 * Return the position of the token of current node.
		 * \return a pair <line,column>
		 */
		virtual std::pair<int,int> getPosition() const;
		
		/**
		 * Print the abstract tree in the given stream.
		 * \param os the ostream where the tree is printed
		 * \param dec the shifting symbol (used for pretty print)
		 */
		virtual void output(std::ostream&,std::string dec="") const;
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
