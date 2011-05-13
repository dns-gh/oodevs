// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_h_
#define __ActionParameter_h_

#include "Parameter_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Displayer_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Parameter
    @brief  Parameter
*/
// Created: SBO 2007-03-19
// =============================================================================
template< typename T >
class Parameter : public Parameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Parameter( const kernel::OrderParameter& parameter );
             Parameter( const kernel::OrderParameter& parameter, const T& value );
    virtual ~Parameter();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetType() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    void SetValue( const T& value );
    const T& GetValue() const;
    bool IsInRange() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Parameter( const Parameter& );            //!< Copy constructor
    Parameter& operator=( const Parameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyValueSet() {};
    virtual std::string SerializeType() const = 0;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    T value_;
    //@}
};

    }
}

#include "Parameter.inl"

#endif // __ActionParameter_h_
