// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/EntityManager.cpp $
// $Author: Sbo $
// $Modtime: 7/07/05 15:09 $
// $Revision: 17 $
// $Workfile: EntityManager.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"

#include "Entities/EntityManager.h"

#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"

#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: EntityManager::EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::EntityManager()
{
    // entities 
    Automat       ::Initialize();
    Pawn          ::Initialize();
    Team          ::Initialize();
    KnowledgeGroup::Initialize();

}

//-----------------------------------------------------------------------------
// Name: EntityManager::~EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    // entity types
    AutomatType   ::Terminate();
    PawnType      ::Terminate();

    // entities
    Automat       ::Terminate();
    Pawn          ::Terminate();
    Team          ::Terminate();
    KnowledgeGroup::Terminate();
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Initialize
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void EntityManager::Initialize( XmlInputArchive& archive ) const
{
    try
    {
        std::string strFile;

        archive.ReadField( "Pions"      , strFile );
        PawnType    ::Initialize( strFile );

        archive.ReadField( "Automates"  , strFile );
        AutomatType ::Initialize( strFile  );
    }
    catch( ... )
    {
        throw;
    }
}
