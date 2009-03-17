// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorSerializer_h_
#define __IndicatorSerializer_h_

#include "IndicatorGrammarHandler_ABC.h"
#include <vector>

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  IndicatorGrammarHandler
    @brief  IndicatorGrammarHandler
*/
// Created: SBO 2009-03-16
// =============================================================================
class IndicatorSerializer : public IndicatorGrammarHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorSerializer( xml::xostream& xos );
    virtual ~IndicatorSerializer();
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value );
    virtual void HandleExtract( const boost::spirit::classic::tree_match< const char* >::const_tree_iterator& it );
    virtual void HandleVariable( const std::string& name );
    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorSerializer( const IndicatorSerializer& );            //!< Copy constructor
    IndicatorSerializer& operator=( const IndicatorSerializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::string T_Parameter;
    //@}

private:
    //! @name Member data
    //@{
    xml::xostream& xos_;
    std::vector< T_Parameter > stack_;
    //@}
};

#endif // __IndicatorSerializer_h_
