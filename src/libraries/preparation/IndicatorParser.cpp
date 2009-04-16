// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorParser.h"
#include "IndicatorGrammarHandler_ABC.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <sstream>

namespace bs = boost::spirit::classic;

// -----------------------------------------------------------------------------
// Name: IndicatorParser constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorParser::IndicatorParser( IndicatorGrammarHandler_ABC& handler )
    : handler_( handler )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
IndicatorParser::~IndicatorParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser::Parse
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorParser::Parse( const std::string& text )
{
    bs::tree_parse_info<> info = bs::ast_parse( text.c_str(), *this, bs::space_p );
    if( !info.full )
    {
        std::stringstream ss;
        ss << "syntax error: " << info.stop;
        throw std::exception( ss.str().c_str() );
    }
    Evaluate( info.trees.begin() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser::Evaluate
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void IndicatorParser::Evaluate( const bs::tree_match< const char* >::const_tree_iterator& it )
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
        throw std::runtime_error( "unknown rule" );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser::EvaluateNumber
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
double IndicatorParser::EvaluateNumber( const bs::node_val_data< const char* >& node ) const
{
    return boost::lexical_cast< double >( EvaluateString( node ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorParser::EvaluateString
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
std::string IndicatorParser::EvaluateString( const bs::node_val_data< const char* >& node ) const
{
    return std::string( node.begin(), node.end() );
}
