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
        case T_Mission_Population_mission_population_manifester : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_manifester; break;
        case T_Mission_Population_mission_population_commettre_exactions : delete pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_commettre_exactions; break;

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
        case eMission_Population_Manifester : CreateMission_Manifester(); break;
        case eMission_Population_CommettreExactions : CreateMission_CommettreExactions(); break;

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
    CreatePoint( asnMission.destination, "Destination", false );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationMissionInterface::CreateMission_Manifester
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::CreateMission_Manifester()
{
    ASN1T_Mission_Population_Manifester& asnMission = *new ASN1T_Mission_Population_Manifester();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Population_mission_population_manifester;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_manifester = &asnMission;
    CreatePoint( asnMission.destination, "Destination", false );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationMissionInterface::CreateMission_CommettreExactions
// Created: AGR
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::CreateMission_CommettreExactions()
{
    ASN1T_Mission_Population_CommettreExactions& asnMission = *new ASN1T_Mission_Population_CommettreExactions();
    pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Population_mission_population_commettre_exactions;
    pASNMsgOrder_->GetAsnMsg().mission.u.mission_population_commettre_exactions = &asnMission;
    CreateLocation( asnMission.zone, "Zone", false );
}

