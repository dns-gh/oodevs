// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Automat.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const sword::AutomatCreation& message,
                  Controller& controller,
                  const kernel::AutomatType& type,
                  actions::ActionsModel& actionsModel,
                  const kernel::Profile_ABC& profile )
    : EntityImplementation< Automat_ABC >( controller, message.automat().id(), QString( message.name().c_str() ), &actionsModel )
    , type_( type )
    , profile_( profile )
{
    if( name_.isEmpty() )
        name_ = QString( type.GetName().c_str() );
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: LDC 2013-04-15
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where,
                    const gui::Viewport_ABC& viewport,
                    gui::GLView_ABC& view ) const
{
    if( !IsAnAggregatedSubordinate() &&
        view.GetCurrentOptions().IsAggregated( *this ) &&
        viewport.IsHotpointVisible() )
        drawable_.Draw( *this, where, viewport, view, -1.5f );
}

// -----------------------------------------------------------------------------
// Name: Automat::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Automat::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AHC 2012-10-11
// -------------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: ABR 2014-08-28
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatAttributes& message )
{
    if( message.has_name() )
        SetName( QString::fromStdString( message.name() ) );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Automat::CanBeRenamed
// Created: LDC 2014-11-14
// -----------------------------------------------------------------------------
bool Automat::CanBeRenamed() const
{
    return profile_.CanDoMagic( *this );
}
