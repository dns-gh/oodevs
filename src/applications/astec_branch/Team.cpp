// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Team.cpp $
// $Author: Ape $
// $Modtime: 16/02/05 15:54 $
// $Revision: 7 $
// $Workfile: Team.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( uint nID, DIN::DIN_Input& input, Controller& controller, KnowledgeGroupFactory_ABC& factory )
    : controller_( controller )
    , factory_( factory )
    , strName_()
    , nID_    ( nID  )
{
    input >> strName_;
}
    
// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup( unsigned int id )
{
    if( ! Resolver< KnowledgeGroup >::Find( id ) )
    {
        KnowledgeGroup* group = factory_.CreateKnowledgeGroup( id );
        Resolver< KnowledgeGroup >::Register( id, *group );
        controller_.Update( *this );
    };
}

// -----------------------------------------------------------------------------
// Name: Team::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long Team::GetId() const
{
    return nID_;
}
    
// -----------------------------------------------------------------------------
// Name: Team::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
std::string Team::GetName() const
{
    return strName_;
}
