// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorVariable_h_
#define __IndicatorVariable_h_

#include "IndicatorConstant.h"

// =============================================================================
/** @class  IndicatorVariable
    @brief  IndicatorVariable
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorVariable : public IndicatorConstant< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorVariable( unsigned long id, const std::string& name, const IndicatorType& type );
    virtual ~IndicatorVariable();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorVariable( const IndicatorVariable& );            //!< Copy constructor
    IndicatorVariable& operator=( const IndicatorVariable& ); //!< Assignment operator
    //@}
};

#endif // __IndicatorVariable_h_
