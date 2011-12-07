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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MedicalCapacity.h"

namespace
{
    PHY_InjuredHuman* GetWound( const DEC_Decision_ABC& wounded )
    {
        const PHY_ComposantePion* majorComposante = wounded.GetPion().GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
        if( !majorComposante )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        PHY_InjuredHuman* injuredHuman = majorComposante->GetInjury();
        return injuredHuman;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::TakeCareOfThePatient
// Created: RFT 2008
// -----------------------------------------------------------------------------
void DEC_MedicalTreatmentFunctions::TakeCareOfThePatient( const MIL_Agent_ABC& /*callerAgent*/, const DEC_Decision_ABC* patient, boost::shared_ptr< DEC_Knowledge_Object > knowledge )
{
    if( !knowledge )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    MIL_Object_ABC* pHospital = knowledge->GetObjectKnown();
    if( !pHospital )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    PHY_InjuredHuman* injuredHuman = GetWound( *patient );
    if( injuredHuman )
        pHospital->Get< MedicalCapacity >().ReceivePatient( *injuredHuman );
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::DetermineHospital
// Created: LDC 2010-07-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_MedicalTreatmentFunctions::DetermineHospital( const MIL_Agent_ABC& caller, const DEC_Decision_ABC* patient, std::vector< boost::shared_ptr< DEC_Knowledge_Object > > hospitals )
{
    typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > >::const_iterator const_iterator;

    PHY_InjuredHuman* injuredHuman = GetWound( *patient );
    double bestDistance = std::numeric_limits< double >::max();
    boost::shared_ptr< DEC_Knowledge_Object > pBestHospital;
    if( injuredHuman )
    {
        const MT_Vector2D& callerPosition = caller.GetRole< PHY_RoleInterface_Location >().GetPosition();
        for( const_iterator it = hospitals.begin(); it != hospitals.end(); ++it )
        {
            if( !(*it) )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
            MIL_Object_ABC* pHospital = (*it)->GetObjectKnown();
            if( pHospital && pHospital->Get< MedicalCapacity >().CanTreat( *pHospital, *injuredHuman ) )
            {
                double distance = pHospital->GetLocalisation().ComputeBarycenter().Distance( callerPosition );
                if( bestDistance > distance )
                {
                    bestDistance = distance;
                    pBestHospital = *it;
                }
            }
        }
    }
    return pBestHospital;
}

// -----------------------------------------------------------------------------
// Name: DEC_MedicalTreatmentFunctions::CanHospitalTreatWound
// Created: LDC 2010-07-01
// -----------------------------------------------------------------------------
bool DEC_MedicalTreatmentFunctions::CanHospitalTreatWound( const MIL_Agent_ABC& /*callerAgent*/, const DEC_Decision_ABC* patient, boost::shared_ptr< DEC_Knowledge_Object > hospital )
{
    PHY_InjuredHuman* injuredHuman = GetWound( *patient );
    if ( !injuredHuman ||!hospital || hospital->GetObjectKnown() )
        return false;
    return hospital->GetObjectKnown()->Get< MedicalCapacity >().CanTreat( *hospital->GetObjectKnown(), *injuredHuman );
}
