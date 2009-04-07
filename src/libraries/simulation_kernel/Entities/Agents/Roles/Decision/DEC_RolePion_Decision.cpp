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
#include "Decision/DEC_Representations.h"
#include "CheckPoints/DIA_Serializer.h"
#include "Network/NET_ASN_Messages.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

int  DEC_RolePion_Decision::nDIAMissionIdx_          = 0; 
int  DEC_RolePion_Decision::nDIANameIdx_             = 0;
int  DEC_RolePion_Decision::nDIAAutomateIdx_         = 0;
uint DEC_RolePion_Decision::nMissionBehaviorDummyId_ = 0;

BOOST_CLASS_EXPORT_GUID( DEC_RolePion_Decision, "DEC_RolePion_Decision" )

//-----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_RolePion_Decision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Pion" );

    nDIANameIdx_      = DEC_Tools::InitializeDIAField( "szName_"   , diaType );
    nDIAMissionIdx_   = DEC_Tools::InitializeDIAField( "mission_"  , diaType );
    nDIAAutomateIdx_  = DEC_Tools::InitializeDIAField( "automate_" , diaType );
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
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );
    
    const DEC_Model_ABC& model = pion.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        GetVariable( nDIANameIdx_      ).SetValue( pion.GetName() );
        GetVariable( nDIAMissionIdx_   ).Reset();
        GetVariable( nDIAAutomateIdx_  ).SetValue( pion.GetAutomate().GetDecision() );
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
        
        std::string strName;
        file >> strName;
        GetVariable( nDIANameIdx_    ).SetValue( strName );
        GetVariable( nDIAMissionIdx_ ).Reset();
        
        DEC_AutomateDecision* pDecision;
        file >> pDecision;
        assert( pDecision );
        GetVariable( nDIAAutomateIdx_  ).SetValue( *pDecision );
        
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
    // $$$$ JVT : Beark
    const std::string diaName = const_cast< DEC_RolePion_Decision& >( *this ).GetVariable( nDIANameIdx_ ).ToString();
    // $$$$ JVT : Beark Arrrg
    DEC_AutomateDecision* const dec = static_cast< DEC_AutomateDecision* >( const_cast< DEC_RolePion_Decision& >( *this ).GetVariable( nDIAAutomateIdx_ ).ToObject() );

    file << boost::serialization::base_object< MT_Role_ABC >( *this )
         << pEntity_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << nIndirectFireAvailability_
         << roe
         << type
         << diaName 
         << dec 
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
// Name: DEC_RolePion_Decision::RemoveRepresentationFromCategory
// Created: LDC 2009-04-06
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RemoveRepresentationFromCategory( const std::string& name, DIA_TypedObject* pObject )
{
    GetRole< DEC_Representations >().RemoveFromCategory( name, pObject );
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
    missionBehaviorParameters_.GetParameter( 0 ).SetValue( mission ); 
    missionBehaviorParameters_.GetParameter( 1 ).SetValue( (int)nMissionBehaviorDummyId_++ ); 
    DIA_ActivateOrder( &GetBehaviorPart(), strBehavior, 1.0, missionBehaviorParameters_ );
    GetVariable( nDIAMissionIdx_ ).SetValue( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StopMissionBehavior( MIL_PionMission& mission )
{
    __try
    {
        const std::string& strBehavior = mission.GetType().GetDIABehavior();
        DIA_DesactivateOrder( &GetBehaviorPart(), strBehavior, missionBehaviorParameters_, true );
        GetVariable( nDIAMissionIdx_ ).Reset();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        CleanStateAfterCrash();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyAutomateChanged
// Created: NLD 2005-04-13
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyAutomateChanged()
{
    assert( pEntity_ );
    GetVariable( nDIAAutomateIdx_ ).SetValue( pEntity_->GetAutomate().GetDecision() );
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

