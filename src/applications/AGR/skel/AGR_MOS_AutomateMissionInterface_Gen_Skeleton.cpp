// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE $TIME$ $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_AutomateMissionInterface_Gen_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 3 $
// $Workfile: AGR_MOS_AutomateMissionInterface_Gen_Skeleton.cpp $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AutomateMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_AutomateMissionInterface::~MOS_AutomateMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
$AsnMissionDeletion$
        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AutomateMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MOS_AutomateMissionInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
$MosMissionCreation$
        default:
            assert( false );
    }

    this->CreateDefaultParameters();
    this->CreateOkCancelButtons();
}


$MissionCreatorImplementations$