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
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#pragma warning( pop )
#pragma warning( disable: 4503 4512 )

//#define BOOST_SPIRIT_DEBUG

namespace bs = boost::spirit::classic;

using namespace indicators;

struct FormulaParser::Impl : public boost::spirit::classic::grammar< FormulaParser::Impl >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Impl( GrammarHandler_ABC& handler );
    virtual ~Impl();
    //@}

    //! @name Operations
    //@{
    void Parse( const std::string& text );
    //@}

    //! @name Helpers
    //@{
    void Evaluate( const boost::spirit::classic::tree_match< const char* >::const_tree_iterator& it );
    double EvaluateNumber( const boost::spirit::classic::node_val_data< const char* >& node ) const;
    std::string EvaluateString( const boost::spirit::classic::node_val_data< const char* >& node ) const;
    //@}

    //! @name Types
    //@{
    enum E_Nodes
    {
        StartTag,
        NumberTag,
        StringTag,
        VariableTag,
        FunctionCallTag,
        FunctionParametersTag,
        FunctionParameterTag
    };
    //@}

public:
    //! @name Types
    //@{
    template< typename T >
    class definition : private boost::noncopyable
    {
    public:
        explicit definition( const FormulaParser::Impl& /*parser*/ )
        {
            number_ = bs::leaf_node_d[ bs::real_p ];
            string_ = bs::no_node_d[ bs::ch_p( '\'' ) ] >> bs::leaf_node_d[ *( bs::anychar_p - bs::ch_p( '\'' ) ) ] >> bs::no_node_d[ bs::ch_p( '\'' ) ];
            constant_ = number_ | string_;

            identifier_ = bs::leaf_node_d[ ( ( bs::alpha_p | '_' ) >> *( bs::alnum_p | '_' | '-' ) ) ];
            variable_   = bs::no_node_d[ bs::ch_p( '$' ) ] >> identifier_;

            functionName_ = identifier_;
            functionCall_ = functionName_ >> bs::no_node_d[ bs::ch_p( '(' ) ] >> !functionParameters_ >> bs::no_node_d[ bs::ch_p( ')' ) ];

            functionParameters_ = bs::list_p( bs::gen_pt_node_d[ functionParameter_ ], bs::no_node_d[ bs::ch_p( ',' ) ] );
            functionParameter_ = bs::gen_pt_node_d[ variable_ ] | constant_ | bs::gen_pt_node_d[ functionCall_ ];

            start_ = bs::gen_pt_node_d[ functionCall_ ] | constant_ | bs::gen_pt_node_d[ variable_ ];

            BOOST_SPIRIT_DEBUG_RULE( start_ );
            BOOST_SPIRIT_DEBUG_RULE( number_ );
            BOOST_SPIRIT_DEBUG_RULE( string_ );
            BOOST_SPIRIT_DEBUG_RULE( variable_ );
            BOOST_SPIRIT_DEBUG_RULE( functionCall_ );
            BOOST_SPIRIT_DEBUG_RULE( functionParameters_ );
            BOOST_SPIRIT_DEBUG_RULE( functionParameter_ );
        }

        const bs::rule< T, bs::parser_context<>, bs::parser_tag< StartTag > >& start() const { return start_; }

    private:
        bs::rule< T > constant_, identifier_, functionName_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< StartTag > >              start_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< NumberTag > >             number_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< StringTag > >             string_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< VariableTag > >           variable_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionCallTag > >       functionCall_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionParametersTag > > functionParameters_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionParameterTag > >  functionParameter_;
    };
    //@}

private:
    //! @name Member data
    //@{
    GrammarHandler_ABC& handler_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: FormulaParser constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
FormulaParser::FormulaParser( GrammarHandler_ABC& handler )
    : pImpl_( new Impl( handler ) )
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
    pImpl_->Parse( text );
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl constructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
FormulaParser::Impl::Impl( GrammarHandler_ABC& handler )
    : handler_( handler )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl destructor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
FormulaParser::Impl::~Impl()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl::Parse
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void FormulaParser::Impl::Parse( const std::string& text )
{
    bs::tree_parse_info<> info = bs::ast_parse( text.c_str(), *this, bs::space_p );
    if( !info.full )
        throw MASA_EXCEPTION( tools::translate( "Indicators", "Syntax error: %1." ).arg( info.stop ).toStdString().c_str() );
    Evaluate( info.trees.begin() );
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl::Evaluate
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
void FormulaParser::Impl::Evaluate( const bs::tree_match< const char* >::const_tree_iterator& it )
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
            const unsigned int paramCount = it->children.size() > 1
                ? static_cast< unsigned int >( it->children[1].children.size() ) : 0;
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
        throw MASA_EXCEPTION( "Unknown rule." );
    }
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl::EvaluateNumber
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
double FormulaParser::Impl::EvaluateNumber( const bs::node_val_data< const char* >& node ) const
{
    return boost::lexical_cast< double >( EvaluateString( node ) );
}

// -----------------------------------------------------------------------------
// Name: FormulaParser::Impl::EvaluateString
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
std::string FormulaParser::Impl::EvaluateString( const bs::node_val_data< const char* >& node ) const
{
    return std::string( node.begin(), node.end() );
}
