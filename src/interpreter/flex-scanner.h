/**
 * \file flex-scanner.h
 * \brief FlexScanner class header.
 * \author Florian Galinier
 * \version 1.1
 * \date 26/04/15
 *
 * Definition of the FlexScanner class inheriting yyFlexLexer.
 *
 */

#pragma once

#undef YY_DECL
#define YY_DECL int stibbons::FlexScanner::yylex()

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "y.tab.h"

namespace stibbons {
	/**
	  * \class FlexScanner
	  * \brief Class for lexical analysis.
	  *
	  * This class contain an overload of yylex function with parameters used
	  * by our lexical parser.
	  *
	  * \author Florian Galinier
	  */
	class FlexScanner : public yyFlexLexer {
	public:
		/**
		 * FlexScanner constructor.
		 * \param in The stream that will be parsed by yylex() method.
		 */
		FlexScanner(std::istream* in) : yyFlexLexer(in) {}
		/**
		 * Return the next token.
		 * \param[out] pyylval The value of the token.
		 * \param[out] loc Location of the token.
		 * \return The last yy::parser::token found or 0 if end of stream.
		 */
		int yylex(yy::parser::semantic_type* pyylval, yy::parser::location_type* loc) { 
			this->pyylval = pyylval; 
			this->loc = loc; 
			return yylex(); 
		}
		
	private:
		/**
		 * Inherited yylex method.
		 * This method is called by public overloaded yylex method.
		 * \return The last yy::parser::token found or 0 if end of stream.
		 */
		int yylex();
		yy::parser::semantic_type* pyylval;
		yy::parser::location_type* loc;
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
