// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************


#include "clients_kernel_pch.h"
#include "Team.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team::Team
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
Team::Team( Controllers& controllers, unsigned long id, const QString& name )
    : EntityImplementation< Team_ABC >( controllers.controller_, id, name )
    , controllers_( controllers )
{

}
    
// -----------------------------------------------------------------------------
// Name: Team::~Team
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
Team::~Team()
{

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
    
// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
void Team::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Team& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Name" ), name_, *this, &Team::Rename );
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: LDC 2012-05-07
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
}
