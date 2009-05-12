// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "FormulaParser.h"
#include "GrammarHandler_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/bind.hpp>
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <boost/spirit/include/classic_ast.hpp>
#pragma warning( disable: 4503 4512 )

namespace bs = boost::spirit::classic;

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: FormulaParser constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
FormulaParser::FormulaParser( GrammarHandler_ABC& handler )
    : handler_( handler )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormulaParser destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
FormulaParser::~FormulaParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Parse
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void FormulaParser::Parse( const std::string& text )
{
    bs::tree_parse_info<> info = bs::ast_parse( text.c_str(), *this, bs::space_p );
    if( !info.full )
        throw std::exception( tools::translate( "Indicators", "Syntax error: %1." ).arg( info.stop ).ascii() );
    Evaluate( info.trees.begin() );
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Evaluate
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void FormulaParser::Evaluate( const bs::tree_match< const char* >::const_tree_iterator& it )
{
    switch( it->value.id().to_long() )
    {
    case StartTag:
        Evaluate( it->children.begin() ); break;
    case NumberTag:
        handler_.HandleNumber( EvaluateNumber( it->value ) ); break;
    case StringTag:
        handler_.HandleString( EvaluateString( it->value ) ); break;
    case VariableTag:
        handler_.HandleVariable( EvaluateString( it->children[0].value ) ); break;
    case FunctionCallTag:
        {
            if( it->children.size() > 1 )
                Evaluate( it->children.begin() + 1 );
            const unsigned int paramCount = it->children.size() > 1 ? it->children[1].children.size() : 0;
            handler_.HandleFunctionCall( EvaluateString( it->children[0].value ), paramCount );
            break;
        }
    case FunctionParametersTag:
        {
            for( bs::tree_match< const char* >::const_tree_iterator i = it->children.begin(); i != it->children.end(); ++i )
                Evaluate( i );
            break;
        }
    case FunctionParameterTag:
        {
            if( it->children.size() > 0 )
                Evaluate( it->children.begin() );
            break;
        }
    default:
        throw std::runtime_error( __FUNCTION__ " unknown rule" );
    }
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::EvaluateNumber
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
double FormulaParser::EvaluateNumber( const bs::node_val_data< const char* >& node ) const
{
    return boost::lexical_cast< double >( EvaluateString( node ) );
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::EvaluateString
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
std::string FormulaParser::EvaluateString( const bs::node_val_data< const char* >& node ) const
{
    return std::string( node.begin(), node.end() );
}
