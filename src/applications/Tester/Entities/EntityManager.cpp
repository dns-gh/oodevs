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

#include "TIC_Pch.h"
#include "Types.h"

#include "Entities/EntityManager.h"

#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Platform.h"

#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"
#include "Entities/Types/PlatformType.h"

#include "Entities/Natures/Pawn/NatureCategory.h"
#include "Entities/Natures/Pawn/NatureLevel.h"
#include "Entities/Natures/Pawn/NatureSpecialization.h"
#include "Entities/Natures/Pawn/NatureWeapon.h"
#include "Entities/Natures/Pawn/NatureMobility.h"
#include "Entities/Natures/Pawn/NatureQualifier.h"
#include "Entities/Natures/Pawn/NatureCapacity.h"

#include "Entities/Natures/Platform/Device.h"
#include "Entities/Natures/Platform/DeviceCategory.h"
#include "Entities/Natures/Platform/DeviceSubCategory.h"

#include "Entities/Formations/FormationManager.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: EntityManager::EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::EntityManager()
{
    // pawn natures
    NatureCategory       ::Initialize();
    NatureLevel          ::Initialize();
    NatureSpecialization ::Initialize();
    NatureWeapon         ::Initialize();
    NatureMobility       ::Initialize();
    NatureQualifier      ::Initialize();
    NatureCapacity       ::Initialize();

    // platform natures
    Device               ::Initialize();
    DeviceCategory       ::Initialize();
    DeviceSubCategory    ::Initialize();

    // entities 
    Automat              ::Initialize();
    Pawn                 ::Initialize();
    Team                 ::Initialize();
    KnowledgeGroup       ::Initialize();

    // formation manager
    FormationManager     ::Initialize();
}

//-----------------------------------------------------------------------------
// Name: EntityManager::~EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    // pawn natures
    NatureCategory       ::Terminate();
    NatureLevel          ::Terminate();
    NatureSpecialization ::Terminate();
    NatureWeapon         ::Terminate();
    NatureMobility       ::Terminate();
    NatureQualifier      ::Terminate();
    NatureCapacity       ::Terminate();

    // platform natures
    Device               ::Terminate();
    DeviceCategory       ::Terminate();
    DeviceSubCategory    ::Terminate();

    // entity types
    AutomatType          ::Terminate();
    PawnType             ::Terminate();
    PlatformType         ::Terminate();

    // entities
    Automat              ::Terminate();
    Pawn                 ::Terminate();
    Team                 ::Terminate();
    KnowledgeGroup       ::Terminate();

    // formation manager
    FormationManager     ::Terminate();
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

        archive.ReadField( "Composantes", strFile );
        PlatformType::Initialize( strFile );

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

//-----------------------------------------------------------------------------
// Name: EntityManager::InitializePlatformNatures
// Created: SBO 2005-07-05
//-----------------------------------------------------------------------------
void EntityManager::InitializePlatformNatures( const std::string& strFile ) const
{
    PlatformType::LoadNatures( strFile );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::SetBlackList
// Created: SBO 2005-06-30
//-----------------------------------------------------------------------------
void EntityManager::SetPlatformTypeFilter( const std::string& strFile ) const
{
    PlatformType::SetBlackList( strFile );
}