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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/FormationManager.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: FormationManager.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Formations/FormationManager.h"
#include "Entities/Formations/Formation_ABC.h"
#include "Entities/Formations/Formation_Line.h"
#include "Entities/Formations/Formation_Road.h"
#include "Entities/Formations/Formation_Diamond.h"

using namespace TIC;

FormationManager::T_FormationMap FormationManager::formationMap_;

//-----------------------------------------------------------------------------
// Name: FormationManager::FormationManager
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
FormationManager::FormationManager()
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: FormationManager::~FormationManager
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
FormationManager::~FormationManager()
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: FormationManager::Initialize
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void FormationManager::Initialize()
{
    formationMap_.insert( std::make_pair( eFormationLine   , new Formation_Line   () ) );
    formationMap_.insert( std::make_pair( eFormationRoad   , new Formation_Road   () ) );
    formationMap_.insert( std::make_pair( eFormationDiamond, new Formation_Diamond() ) );
}

//-----------------------------------------------------------------------------
// Name: FormationManager::Terminate
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void FormationManager::Terminate()
{
    for( CIT_FormationMap it = formationMap_.begin(); it != formationMap_.end(); ++it )
        delete it->second;
    formationMap_.clear();
}