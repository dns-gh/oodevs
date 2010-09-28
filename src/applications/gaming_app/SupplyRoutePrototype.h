// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRoutePrototype_h_
#define __SupplyRoutePrototype_h_

#include "clients_gui/SupplyRoutePrototype_ABC.h"
#include "protocol/clientsenders.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

struct ObjectAttributesLogisticRoute;

// =============================================================================
/** @class  SupplyRoutePrototype
    @brief  SupplyRoutePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class SupplyRoutePrototype : public gui::SupplyRoutePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SupplyRoutePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~SupplyRoutePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyRoutePrototype( const SupplyRoutePrototype& );            //!< Copy constructor
    SupplyRoutePrototype& operator=( const SupplyRoutePrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    ObjectAttributesLogisticRoute* attr_;
    //@}
};

#endif // __SupplyRoutePrototype_h_
