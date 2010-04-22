// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.cpp $
// $Author: Rft $
// $Modtime: 2/05/05 16:22 $
// $Revision: 9 $
// $Workfile: DEC_FireFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_MedicalTreatmentFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"

#include "Entities/Objects/MedicalTreatmentAttribute.h"

#include "Knowledge/DEC_KS_ObjectInteraction.h"

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::TakeCareOfThePatient
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_MedicalTreatmentFunctions::TakeCareOfThePatient( MIL_AgentPion& /*callerAgent*/ )
{
    //Deposer le malade dans une structure medicale (hopital ou autre)
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::PutPatientInTransport
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_MedicalTreatmentFunctions::PutPatientInTransport( MIL_AgentPion& /*callerAgent*/ )
{
    //Mettre le malade dans le transport (ambulance, camion de pompier ou autre)
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::GetMedicalTreatmentTypes
// Created: RFT 2008
// -----------------------------------------------------------------------------
int DEC_MedicalTreatmentFunctions::GetMedicalTreatmentTypes( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    assert( knowledge );
    MIL_Object_ABC* pObject = knowledge->GetObjectKnown();
    
    if( !pObject || pObject->IsMarkedForDestruction() )
    {
        return 0;
    }

    callerAgent.GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );

    int id ( 0 );
    //ICI donner la liste des ID des services medicaux que l on peut traiter
    //call.GetResult().SetValue( ( int ) object.GetAttribute< MedicalTreatmentAttribute >().GetMap.GetID() );
    return id;
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::DoDiagnosis
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_MedicalTreatmentFunctions::DoDiagnosis( MIL_AgentPion& /*callerAgent*/ )
{
    //Pour un injuredHUman, regarder le nom de sa blessure ou son ID    
}