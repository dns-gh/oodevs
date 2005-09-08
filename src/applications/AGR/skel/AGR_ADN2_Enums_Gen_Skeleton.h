// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ADN2_Enums_Gen_Skeleton.h $
// $Author: Ape $
// $Modtime: 18/02/05 16:06 $
// $Revision: 1 $
// $Workfile: AGR_ADN2_Enums_Gen_Skeleton.h $
//
// *****************************************************************************

#ifndef __ADN_Enums_Gen_Skeleton_h_
#define __ADN_Enums_Gen_Skeleton_h_

// Sorted missions enumeration
enum E_SMission
{
    // 
    // MISSIONS PION
    // 

    // Common
    eSMission_Pion_Common_Debut,
$CommonMissionPion$
    eSMission_Pion_Common_Fin,

    // ABC
    eSMission_Pion_ABC_Debut,
$AbcMissionPion$
    eSMission_Pion_ABC_Fin,
          
    // INF
    eSMission_Pion_INF_Debut,
$InfMissionPion$
    eSMission_Pion_INF_Fin,
    
    // GEN
    eSMission_Pion_GEN_Debut,
$GenMissionPion$
    eSMission_Pion_GEN_Fin,
    
    // 3D - ASS
    eSMission_Pion_3D_Debut,
$3dMissionPion$
    eSMission_Pion_3D_Fin,

    // 3D - ALAT
    eSMission_Pion_ALAT_Debut,
$AlatMissionPion$
    eSMission_Pion_ALAT_Fin,

    // LOG - SANTE
    eSMission_Pion_LOG_Debut,
$LogMisssionPion$
    eSMission_Pion_LOG_Fin,

    // RENS
    eSMission_Pion_RENS_Debut,
$RensMisssionPion$
    eSMission_Pion_RENS_Fin,

    // NBC
    eSMission_Pion_NBC_Debut,
$NbcMisssionPion$
    eSMission_Pion_NBC_Fin,

    // MILICE
    eSMission_Pion_MILICE_Debut,
$MiliceMisssionPion$
    eSMission_Pion_MILICE_Fin,

    // JOINT
    eSMission_Pion_JOINT_Debut,
$JointMisssionPion$
    eSMission_Pion_JOINT_Fin,

    // 
    // MISSIONS AUTOMATE
    // 

    // Common
    eSMission_Automate_Common_Debut,
$CommonMissionAutomate$
    eSMission_Automate_Common_Fin,

    // ABC
    eSMission_Automate_ABC_Debut,
$AbcMissionAutomate$
    eSMission_Automate_ABC_Fin,

    // INF
    eSMission_Automate_INF_Debut,
$InfMissionAutomate$
    eSMission_Automate_INF_Fin,

    // GEN                                               
    eSMission_Automate_GEN_Debut,
$GenMissionAutomate$
    eSMission_Automate_GEN_Fin,

    // LOG : SANTE
    eSMission_Automate_LOG_Debut,
$LogMissionAutomate$
    eSMission_Automate_LOG_Fin,

    // ASS
    eSMission_Automate_3D_Debut,
$3dMissionAutomate$
    eSMission_Automate_3D_Fin,

    // ALAT
    eSMission_Automate_ALAT_Debut,
$AlatMissionAutomate$
    eSMission_Automate_ALAT_Fin,

    // RENS
    eSMission_Automate_RENS_Debut,
$RensMisssionAutomate$
    eSMission_Automate_RENS_Fin,

    // NBC
    eSMission_Automate_NBC_Debut,
$NbcMisssionAutomate$
    eSMission_Automate_NBC_Fin,

    // MILICE
    eSMission_Automate_MILICE_Debut,
$MiliceMisssionAutomate$
    eSMission_Automate_MILICE_Fin,

    // NBC
    eSMission_Automate_JOINT_Debut,
$JointMisssionAutomate$
    eSMission_Automate_JOINT_Fin,

    eNbrSMission,

$TestMission$

    eNbrSMissionTest
};

#endif // __ADN_Enums_Gen_Skeleton_h_
