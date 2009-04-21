// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:28 $
// $Revision: 19 $
// $Workfile: DEC_RolePion_Decision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_RolePion_Decision.h"

#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "CheckPoints/DIA_Serializer.h"
#include "Network/NET_ASN_Messages.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

BOOST_CLASS_EXPORT_GUID( DEC_RolePion_Decision, "DEC_RolePion_Decision" )

//-----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_RolePion_Decision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Pion" );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::DEC_RolePion_Decision( MT_RoleContainer& role, MIL_AgentPion& pion )
    : DEC_Decision              ( pion, "T_Pion" )
    , diaFunctionCaller_        ( pion, pion.GetType().GetFunctionTable() )
    , nForceRatioState_         ( eForceRatioStateNone         )
    , nRulesOfEngagementState_  ( eRoeStateNone                )
    , nCloseCombatState_        ( eCloseCombatStateNone        )
    , nOperationalState_        ( eOpStateOperational          )
    , nIndirectFireAvailability_( eFireAvailabilityNone        )
    , pRoePopulation_           ( &PHY_RoePopulation::none_    )
    , bStateHasChanged_         ( true                         )
    , pAutomate_                ( 0                            )
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );
    
    const DEC_Model_ABC& model = pion.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        name_ = pion.GetName();
        pAutomate_ = &pion.GetAutomate().GetDecision();
        DIA_Workspace::Instance().SetObjectName( *this, pion.GetName() ); // ????
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    // Parameters list for calling mission behavior
    missionBehaviorParameters_.SetOwnerShip( true );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    // Parameters list for calling default behavior
    StartDefaultBehavior();    
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::DEC_RolePion_Decision()
    : DEC_Decision              ( "T_Pion" ) 
    , diaFunctionCaller_        ( *(MIL_AgentPion*)0, *(DIA_FunctionTable< MIL_AgentPion >*)1 ) // $$$$ JVT : Eurkkk
    , nForceRatioState_         ( eForceRatioStateNone      )
    , nRulesOfEngagementState_  ( eRoeStateNone             )
    , nCloseCombatState_        ( eCloseCombatStateNone     )
    , nOperationalState_        ( eOpStateOperational       )
    , nIndirectFireAvailability_( eFireAvailabilityNone     )
    , pRoePopulation_           ( &PHY_RoePopulation::none_ )
    , bStateHasChanged_         ( true                      )
    , pAutomate_                ( 0                            )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::~DEC_RolePion_Decision()
{
    StopDefaultBehavior();
    MT_DELETEOWNED( missionBehaviorParameters_.GetParameters() );
}


// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MT_Role_ABC >( *this )
         >> pEntity_ 
         >> nForceRatioState_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_
         >> nIndirectFireAvailability_;
    assert( pEntity_ );

    uint nRoePopulationID;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );
    assert( pRoePopulation_ );
       
    uint nPionTypeID;
    file >> nPionTypeID;
          
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nPionTypeID );
    assert( pType );    

    diaFunctionCaller_.DIA_FunctionCaller< MIL_AgentPion >::DIA_FunctionCaller( *pEntity_, pType->GetFunctionTable() );

    RegisterUserFunctionCaller( diaFunctionCaller_ );
 
    const DEC_Model_ABC& model = pType->GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        
        file >> name_;
                
        DEC_AutomateDecision* pDecision;
        file >> pDecision;
        assert( pDecision );
        pAutomate_ = pDecision;
        
        DIA_Workspace::Instance().SetObjectName( *this, pEntity_->GetName() ); // ????
        
        DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
        file >> diaSerializer;
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }
 
    // Parameters list for calling mission behavior
    missionBehaviorParameters_.SetOwnerShip( true );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    // Parameters list for calling default behavior
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    unsigned roe  = pRoePopulation_->GetID(),
             type = pEntity_->GetType().GetID();
    
    file << boost::serialization::base_object< MT_Role_ABC >( *this )
         << pEntity_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << nIndirectFireAvailability_
         << roe
         << type
         << name_ 
         << pAutomate_ 
         << diaSerializer;         
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::EndCleanStateAfterCrash
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::EndCleanStateAfterCrash()
{
    pEntity_->CancelAllActions();
    GetRole< PHY_RolePion_Perceiver >().DisableAllPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RemoveAllReferencesOf
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RemoveAllReferencesOf( const DIA_TypedObject& referenced, DIA_ExecutionContext& context )
{
    GetBehaviorPart().RemoveAllReferencesOf( referenced, context );
}

// =============================================================================
// DEFAULT BEHAVIOR MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StartMissionBehavior( MIL_PionMission& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    ActivateOrder( strBehavior, missionBehaviorParameters_, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StopMissionBehavior( MIL_PionMission& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    StopMission( strBehavior, missionBehaviorParameters_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyAutomateChanged
// Created: NLD 2005-04-13
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyAutomateChanged()
{
    assert( pEntity_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRoePopulationChanged
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyRoePopulationChanged( const PHY_RoePopulation& roe )
{
    assert( pRoePopulation_ );
    if( roe != *pRoePopulation_ )
    {
        pRoePopulation_   = &roe;
        bStateHasChanged_ = true;
    }
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    assert( pRoePopulation_ );

    msg().m.rapport_de_forcePresent              = 1;    
    msg().m.combat_de_rencontrePresent           = 1;
    msg().m.etat_operationnelPresent             = 1;
    msg().m.disponibilite_au_tir_indirectPresent = 1;
    msg().m.roePresent                           = 1;
    msg().m.roe_populationPresent                = 1;

    msg().rapport_de_force              = (ASN1T_EnumForceRatioStatus)nForceRatioState_;
    msg().combat_de_rencontre           = (ASN1T_EnumMeetingEngagementStatus)nCloseCombatState_;
    msg().etat_operationnel             = (ASN1T_EnumOperationalStatus)nOperationalState_;
    msg().disponibilite_au_tir_indirect = (ASN1T_EnumFireAvailability)nIndirectFireAvailability_;
    msg().roe                           = (ASN1T_EnumRoe)nRulesOfEngagementState_;
    msg().roe_population                = pRoePopulation_->GetAsnID();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

