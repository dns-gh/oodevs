// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Team.h"
#include "IdManager.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controllers& controllers, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controllers.controller_, idManager.GetNextId(), "" )
    , controllers_( controllers )
{
    name_ = tools::translate( "Preparation", "Army %1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, Controllers& controllers, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controllers.controller_, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , controllers_( controllers )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controllers.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Team::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Team::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_.ascii() );
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void Team::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Team& constSelf = *this;
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Team", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Team", "Info/Name" ), name_, *this, &Team::Rename );
}

// -----------------------------------------------------------------------------
// Name: Team::OptionChanged
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
void Team::OptionChanged( const std::string& name, const kernel::OptionVariant& /*value*/ )
{
    if( name == "Color/Phantom" )
    {
        if( const kernel::TacticalHierarchies* pTactical = Retrieve< kernel::TacticalHierarchies >() )
            controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
}
