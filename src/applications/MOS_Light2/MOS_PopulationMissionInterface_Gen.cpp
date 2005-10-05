// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE  $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MOS_PopulationMissionInterface_Gen_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 3 $
// $Workfile: AGR_MOS_PopulationMissionInterface_Gen_Skeleton.cpp $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_PopulationMissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_PopulationMissionInterface::~MOS_PopulationMissionInterface()
{
    switch( pASNMsgOrder_->GetAsnMsg().mission.t )
    {
        case T_Mission_Population_mission_population_faire_mouvement : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_faire_mouvement; break;

        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationMissionInterface::CreateInterface
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::CreateInterface()
{
    switch( nMissionId_ )
    {
        case eMission_Population_FaireMouvement : CreateMission_FaireMouvement(); break;

        default:
            assert( false );
    }

    this->CreateDefaultParameters();
    this->CreateOkCancelButtons();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationMissionInterface::CreateMission_FaireMouvement
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::CreateMission_FaireMouvement()
{
    ASN1T_Mission_Population_FaireMouvement& asnMission = *new ASN1T_Mission_Population_FaireMouvement();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Population_mission_population_faire_mouvement;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_faire_mouvement = &asnMission;
    CreatePoint( asnMission.destination, "Destination" );
}

