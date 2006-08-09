// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "astec_kernel/Controller.h"

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
    InterfaceContainer< Extension_ABC >::Register( *this );
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
    if( ! Resolver< KnowledgeGroup >::Find( id ) )
    {
        KnowledgeGroup* group = factory_.CreateKnowledgeGroup( id, *this );
        Resolver< KnowledgeGroup >::Register( id, *group );
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
