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

namespace kernel
{
    class Object_ABC;
}

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
             SupplyRoutePrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~SupplyRoutePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyRoutePrototype( const SupplyRoutePrototype& );            //!< Copy constructor
    SupplyRoutePrototype& operator=( const SupplyRoutePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __SupplyRoutePrototype_h_
