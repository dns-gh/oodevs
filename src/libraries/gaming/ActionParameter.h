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

#include "ActionParameter_ABC.h"
#include "Tools.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Displayer_ABC.h"
#include "xeumeuleu/xml.h"

// =============================================================================
/** @class  ActionParameter
    @brief  ActionParameter
*/
// Created: SBO 2007-03-19
// =============================================================================
template< typename T >
class ActionParameter : public ActionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameter( const kernel::OrderParameter& parameter );
             ActionParameter( const kernel::OrderParameter& parameter, const T& value );
    virtual ~ActionParameter();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetType() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    void SetValue( const T& value );
    const T& GetValue() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameter( const ActionParameter& );            //!< Copy constructor
    ActionParameter& operator=( const ActionParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    T value_;
    //@}
};

#include "ActionParameter.inl"

#endif // __ActionParameter_h_
