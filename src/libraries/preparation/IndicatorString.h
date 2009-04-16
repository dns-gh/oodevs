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
    @brief  IndicatorString
*/
// Created: SBO 2009-04-16
// =============================================================================
class IndicatorString : public IndicatorConstant< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorString( const std::string& value, const IndicatorType& type );
    virtual ~IndicatorString();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorString( const IndicatorString& );            //!< Copy constructor
    IndicatorString& operator=( const IndicatorString& ); //!< Assignment operator
    //@}
};

#endif // __IndicatorString_h_
