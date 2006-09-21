// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRouteAttributes_h_
#define __LogisticRouteAttributes_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

// =============================================================================
/** @class  LogisticRouteAttributes
    @brief  LogisticRouteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticRouteAttributes : public kernel::LogisticRouteAttributes_ABC
                              , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRouteAttributes();
    virtual ~LogisticRouteAttributes();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetFlow( unsigned int value );
    void SetWidth( unsigned int value );
    void SetLength( unsigned int value );
    void SetMaxWeight( unsigned int value );
    void SetEquipped( bool value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRouteAttributes( const LogisticRouteAttributes& );            //!< Copy constructor
    LogisticRouteAttributes& operator=( const LogisticRouteAttributes& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::OptionalValue< unsigned > flow_;
    kernel::OptionalValue< unsigned > width_;
    kernel::OptionalValue< unsigned > length_;
    kernel::OptionalValue< unsigned > maxWeight_;
    kernel::OptionalValue< bool > equipped_;
    //@}
};

#endif // __LogisticRouteAttributes_h_
