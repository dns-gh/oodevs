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
Team::Team( uint id, DIN::DIN_Input& input, Controller& controller )
    : controller_( controller )
    , id_( id )
{
    RegisterSelf( *this );
    std::string name;
    input >> name;
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    controller_.Delete( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void Team::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *(Team_ABC*)this );
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
QString Team::GetName() const
{
    return name_;
}
