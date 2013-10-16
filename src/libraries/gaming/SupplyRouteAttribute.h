// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyRouteAttribute : public kernel::SupplyRouteAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRouteAttribute( kernel::Controller& controller );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyRouteAttribute( const SupplyRouteAttribute& );            //!< Copy constructor
    SupplyRouteAttribute& operator=( const SupplyRouteAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::OptionalValue< unsigned > nLogRouteFlow_;
    kernel::OptionalValue< unsigned > nLogRouteWidth_;
    kernel::OptionalValue< unsigned > nLogRouteLength_;
    kernel::OptionalValue< unsigned > nLogRouteMaxWeight_;
    kernel::OptionalValue< bool > bLogRouteEquipped_;
    //@}
};

#endif // __SupplyRouteAttribute_h_
