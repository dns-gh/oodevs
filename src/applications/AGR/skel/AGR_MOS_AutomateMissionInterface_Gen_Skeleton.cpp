// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
AutomateMissionInterface::~AutomateMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
$AsnMissionDeletion$
        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateInterface()
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