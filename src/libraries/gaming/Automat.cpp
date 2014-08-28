// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Automat.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_gui/AggregatedTools.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const sword::AutomatCreation& message,
                  Controller& controller,
                  const kernel::AutomatType& type,
                  const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : EntityImplementation< Automat_ABC >( controller, message.automat().id(), QString( message.name().c_str() ), canBeRenamedFunctor )
    , type_( type )
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
void Automat::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !IsAggregated() && HasAggregatedSubordinate( *this ) && viewport.IsHotpointVisible() )
        drawable_.Draw( *this, where, viewport, tools, -1.5f );
}

// -----------------------------------------------------------------------------
// Name: Automat::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Automat::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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
        name_ = QString::fromStdString( message.name() );
    Touch();
}
