// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Formation.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const sword::FormationCreation& message,
                      Controller& controller,
                      const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : EntityImplementation< Formation_ABC >( controller, message.formation().id(), QString( message.name().c_str() ), canBeRenamedFunctor )
    , controller_   ( controller )
    , level_        ( static_cast< E_NatureLevel >( message.level() ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( ENT_Tr::ConvertFromNatureLevel( level_ ).c_str() ).arg( message.formation().id() );
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
E_NatureLevel Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Draw
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
void Formation::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& view ) const
{
    if( !IsAnAggregatedSubordinate() && view.GetOptions().IsAggregated( *this ) && viewport.IsHotpointVisible() )
        drawable_.Draw( *this, where, viewport, view, -2.f );
}

// -----------------------------------------------------------------------------
// Name: Formation::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Formation::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const sword::FormationUpdate& message )
{
    if( message.has_name() )
        SetName( QString::fromStdString( message.name() ) );
    Touch();
}
