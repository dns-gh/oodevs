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

namespace xml
{
    class xistream;
}

class IndicatorType;

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
             IndicatorVariable( const std::string& name, const std::string& type, const std::string& value );
    explicit IndicatorVariable( xml::xistream& xis );
    virtual ~IndicatorVariable();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorVariable( const IndicatorVariable& );            //!< Copy constructor
    IndicatorVariable& operator=( const IndicatorVariable& ); //!< Assignment operator
    //@}
};

#endif // __IndicatorVariable_h_
