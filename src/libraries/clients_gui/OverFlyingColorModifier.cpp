// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OverFlyingColorModifier.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: OverFlyingColorModifier constructor
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
OverFlyingColorModifier::OverFlyingColorModifier( kernel::Controllers& controllers )
    : controllers_    ( controllers )
    , overFlownEntity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OverFlyingColorModifier destructor
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
OverFlyingColorModifier::~OverFlyingColorModifier()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OverFlyingColorModifier::Apply
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
QColor OverFlyingColorModifier::Apply( const kernel::Entity_ABC& entity, const QColor& base )
{
    if( overFlownEntity_ && overFlownEntity_ == &entity )
        return base.light( 150 );
    return base;
}

// -----------------------------------------------------------------------------
// Name: OverFlyingColorModifier::NotifyOverFlown
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
void OverFlyingColorModifier::NotifyOverFlown( const kernel::Entity_ABC* element )
{
    overFlownEntity_ = element;
}
