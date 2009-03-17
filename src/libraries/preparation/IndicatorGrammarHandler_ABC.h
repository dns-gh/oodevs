// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorGrammarHandler_ABC_h_
#define __IndicatorGrammarHandler_ABC_h_

#include <boost/spirit/include/classic_parse_tree.hpp>

// =============================================================================
/** @class  IndicatorGrammarHandler_ABC
    @brief  IndicatorGrammarHandler_ABC
*/
// Created: SBO 2009-03-16
// =============================================================================
class IndicatorGrammarHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorGrammarHandler_ABC() {}
    virtual ~IndicatorGrammarHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value ) = 0;
    virtual void HandleExtract( const boost::spirit::classic::tree_match< const char* >::const_tree_iterator& it ) = 0;
    virtual void HandleVariable( const std::string& name ) = 0;
    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorGrammarHandler_ABC( const IndicatorGrammarHandler_ABC& );            //!< Copy constructor
    IndicatorGrammarHandler_ABC& operator=( const IndicatorGrammarHandler_ABC& ); //!< Assignment operator
    //@}
};

#endif // __IndicatorGrammarHandler_ABC_h_
