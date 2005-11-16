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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Population.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:06 $
// $Revision: 38 $
// $Workfile: Population.cpp $
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Entities/Population.h"
#include "Tools/Position.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Population_Type.h"
#include "Workspace.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: Population::Population
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Population::Population( const Workspace& workspace, const ASN1T_MsgPopulationCreation& asnMsg )
    : Testable_Entity ( workspace )
    , nId_            ( asnMsg.oid_population )
    , strName_        ( asnMsg.nom )
{
}

//-----------------------------------------------------------------------------
// Name: Population::~Population
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Population::~Population()
{
}

// -----------------------------------------------------------------------------
// Name: Population::SendMagicAction
// Created: SBO 2005-09-01
// -----------------------------------------------------------------------------
void Population::SendMagicAction( int action ) const
{
}

//-----------------------------------------------------------------------------
// SCHEDULING
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: Population::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Population::ScheduleAllMissions( Scheduler& scheduler, uint nIteration /* = 1 */ )
{
//    assert( pType_ );
//    pType_->GetModel().ScheduleAllMissions( *this, scheduler, nIteration );
}

// -----------------------------------------------------------------------------
// Name: Population::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Population::ScheduleMission( Scheduler& scheduler, const std::string& strMissionName, uint nIteration /* = 1 */ )
{
//    assert( pType_ );
//    pType_->GetModel().ScheduleMission( *this, scheduler, strMissionName, nIteration );
}

