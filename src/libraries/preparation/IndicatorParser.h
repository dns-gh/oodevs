// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorParser_h_
#define __IndicatorParser_h_

//#define BOOST_SPIRIT_DEBUG

#pragma warning( push, 0 )
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#pragma warning( pop )

namespace bs = boost::spirit::classic;

class IndicatorGrammarHandler_ABC;

// =============================================================================
/** @class  IndicatorParser
    @brief  IndicatorParser
*/
// Created: SBO 2009-03-16
// =============================================================================
class IndicatorParser : public boost::spirit::classic::grammar< IndicatorParser >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorParser( IndicatorGrammarHandler_ABC& handler );
    virtual ~IndicatorParser();
    //@}

    //! @name Operations
    //@{
    void Parse( const std::string& text );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorParser( const IndicatorParser& );            //!< Copy constructor
    IndicatorParser& operator=( const IndicatorParser& ); //!< Assignment operator
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
    //! @name Parser
    //@{
    template< typename T >
    class definition
    {
    public:
        explicit definition( const IndicatorParser& parser )
        {
            number_ = bs::leaf_node_d[ bs::real_p ];
//            string_ = bs::confix_p( "'", bs::leaf_node_d[ *bs::alpha_p ], "'" );
            string_ = bs::no_node_d[ bs::ch_p( '\'' ) ] >> bs::leaf_node_d[ *bs::alpha_p ] >> bs::no_node_d[ bs::ch_p( '\'' ) ];
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

        virtual ~definition()
        {
        }

        const bs::rule< T, bs::parser_context<>, bs::parser_tag< StartTag > >& start() const { return start_; }

    private:
        //! @name Copy/Assignment
        //@{
        definition& operator=( const definition& );
        definition( const definition& );
        //@}

        //! @name Member data
        //@{
        bs::rule< T > constant_, identifier_, functionName_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< StartTag > >              start_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< NumberTag > >             number_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< StringTag > >             string_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< VariableTag > >           variable_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionCallTag > >       functionCall_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionParametersTag > > functionParameters_;
        bs::rule< T, bs::parser_context<>, bs::parser_tag< FunctionParameterTag > >  functionParameter_;
        //@}
    };
    //@}

private:
    //! @name Member data
    //@{
    IndicatorGrammarHandler_ABC& handler_;
    //@}
};

#endif // __IndicatorParser_h_
