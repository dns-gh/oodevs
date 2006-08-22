// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Team.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( uint id, DIN::DIN_Input& input, Controller& controller, KnowledgeGroupFactory_ABC& factory )
    : controller_( controller )
    , factory_( factory )
    , name_()
    , id_( id )
{
    RegisterSelf( *this );
    input >> name_;
    controller_.Create( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    controller_.Delete( *(Team_ABC*)this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Team::Update
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team::DoUpdate( const KnowledgeGroupCreationMessage& message )
{
    unsigned long id;
    message >> id;
    if( ! Resolver< KnowledgeGroup_ABC >::Find( id ) )
    {
        KnowledgeGroup_ABC* group = factory_.CreateKnowledgeGroup( id, *this );
        Resolver< KnowledgeGroup_ABC >::Register( id, *group );
        controller_.Update( *(Team_ABC*)this );
    };
}

// -----------------------------------------------------------------------------
// Name: Team::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long Team::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Team::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
std::string Team::GetName() const
{
    return name_;
}
