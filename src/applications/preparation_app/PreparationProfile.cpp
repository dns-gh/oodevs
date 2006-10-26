// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PreparationProfile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: PreparationProfile constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PreparationProfile::PreparationProfile( QObject* parent, kernel::Controllers& controllers )
    : QObject( parent )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PreparationProfile::~PreparationProfile()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::NotifyUpdated
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void PreparationProfile::NotifyUpdated( const kernel::ModelLoaded& )
{
    controllers_.controller_.Update( *(Profile_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::IsVisible
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::IsVisible( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::CanBeOrdered
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::CanBeOrdered( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::CanDoMagic
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::CanDoMagic( const kernel::Entity_ABC& ) const
{
    return true;
}
