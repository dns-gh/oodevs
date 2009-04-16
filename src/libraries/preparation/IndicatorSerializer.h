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

class IndicatorElementFactory_ABC;
class IndicatorElement_ABC;

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
             IndicatorSerializer( const IndicatorElementFactory_ABC& factory, xml::xostream& xos );
    virtual ~IndicatorSerializer();
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value );
    virtual void HandleString( const std::string& value );
    virtual void HandleVariable( const std::string& name );
    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorSerializer( const IndicatorSerializer& );            //!< Copy constructor
    IndicatorSerializer& operator=( const IndicatorSerializer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const IndicatorElementFactory_ABC& factory_;
    xml::xostream& xos_;
    std::vector< boost::shared_ptr< IndicatorElement_ABC > > stack_;
    //@}
};

#endif // __IndicatorSerializer_h_
