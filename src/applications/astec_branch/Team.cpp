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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Team.h"

#include "App.h"
#include "Gtia.h"
#include "Tools.h"
#include "ObjectKnowledge.h"
#include "Object_ABC.h"
// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::Team()
{
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( uint nID, DIN::DIN_Input& input )
: strName_              ()
, nID_                  ( nID  )
{
    input >> strName_;
}
    
// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    this->DestroyAllObjectKnowledges();
//    this->DeleteAllGtias();

}

// -----------------------------------------------------------------------------
// Name: Team::DestroyAllObjectKnowledges
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
void Team::DestroyAllObjectKnowledges()
{
    for( CIT_ObjectKnowledgeMap it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        delete it->second;
    objectKnowledges_.clear();
}

// -----------------------------------------------------------------------------
// Name: Team::CreateGtia
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
Gtia* Team::CreateGtia( uint32 nId )
{
    if( FindGtia( nId ) )
        return FindGtia( nId );
    Gtia* gtia = new Gtia( nId );
    RegisterGtia( *gtia );
    return gtia;
}

