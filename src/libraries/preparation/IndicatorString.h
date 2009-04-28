// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorString_h_
#define __IndicatorString_h_

#include "IndicatorConstant.h"

// =============================================================================
/** @class  IndicatorString
    @brief  Indicator string literal element
*/
// Created: SBO 2009-04-16
// =============================================================================
class IndicatorString : public IndicatorConstant< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IndicatorString( const std::string& value );
    virtual ~IndicatorString();
    //@}

    //! @name Operations
    //@{
    virtual IndicatorElement_ABC& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorString( const IndicatorString& );            //!< Copy constructor
    IndicatorString& operator=( const IndicatorString& ); //!< Assignment operator
    //@}
};

#endif // __IndicatorString_h_
