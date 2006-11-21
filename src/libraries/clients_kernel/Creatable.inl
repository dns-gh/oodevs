// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Controller.h"

namespace kernel
{
// -----------------------------------------------------------------------------
// Name: Creatable constructor
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Iface >
Creatable< Iface >::Creatable( Controller& controller, typename Iface* _this )
     : controller_( controller )
     , this_( _this )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Creatable destructor
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Iface >
Creatable< Iface >::~Creatable()
{
    controller_.Delete( *this_ );
}

// -----------------------------------------------------------------------------
// Name: Creatable::DoUpdate
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
template< typename Iface >
void Creatable< Iface >::DoUpdate( const InstanciationComplete& )
{
    controller_.Create( *this_ );
}

}