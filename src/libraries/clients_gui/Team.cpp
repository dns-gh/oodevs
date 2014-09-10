// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Team.h"
#include "PropertiesDictionary.h"

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Team::Team
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
Team::Team( Controllers& controllers,
            unsigned long id,
            const QString& name,
            const T_CanBeRenamedFunctor& canBeRenamedFunctor /* = T_CanBeRenamedFunctor() */ )
    : EntityImplementation< Team_ABC >( controllers.controller_, id, name, canBeRenamedFunctor )
    , controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::~Team
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
Team::~Team()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team::OptionChanged
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
void Team::OptionChanged( const std::string& name, const kernel::OptionVariant& )
{
    if( name == "Color/Neutralized" || name == "Color/TacticallyDestroyed" || name == "Color/TotallyDestroyed" || name == "Color/Phantom" )
    {
        if( const kernel::TacticalHierarchies* pTactical = Retrieve< kernel::TacticalHierarchies >() )
            controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
}
