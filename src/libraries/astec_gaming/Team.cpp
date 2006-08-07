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

#include "astec_gaming_pch.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "astec_kernel/Controller.h"

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
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    controller_.Delete( *this );
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
        KnowledgeGroup* group = factory_.CreateKnowledgeGroup( id, *this );
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

// -----------------------------------------------------------------------------
// Name: Team::operator==
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool Team::operator==( const Team& team ) const
{
    return this == &team;
}

// -----------------------------------------------------------------------------
// Name: Team::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void Team::Select( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02:
}

// -----------------------------------------------------------------------------
// Name: Team::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void Team::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    // $$$$ SBO 2006-08-02: 
}

// -----------------------------------------------------------------------------
// Name: Team::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void Team::Activate( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02: 
}
