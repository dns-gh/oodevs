// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
UnitMissionInterface::~UnitMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
$AsnMissionDeletion$
        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateInterface()
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