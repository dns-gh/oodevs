// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "Param_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: Param_ABC constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Param_ABC::Param_ABC( const QString& name )
    : name_( name )
    , controller_( 0 )
    , listener_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Param_ABC::~Param_ABC()
{
    RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RemoveFromController
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::RemoveFromController()
{
    if( controller_ )
        controller_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RegisterIn
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void Param_ABC::RegisterIn( kernel::ActionController& controller )
{
    controller.Register( *this );
    controller_ = &controller;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Param_ABC::Draw( const geometry::Point2f&, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::CheckValidity
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
bool Param_ABC::CheckValidity()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::GetName
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QString Param_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsOptional
// Created: SBO 2008-03-06
// -----------------------------------------------------------------------------
bool Param_ABC::IsOptional() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::RegisterListener
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::RegisterListener( Param_ABC& param )
{
    listener_ = &param;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::NotifyChange
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::NotifyChange()
{
    if( listener_ )
        listener_->NotifyChanged( *this );
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::NotifyChanged
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void Param_ABC::NotifyChanged( Param_ABC& /*param*/ )
{
    // NOTHING
}
