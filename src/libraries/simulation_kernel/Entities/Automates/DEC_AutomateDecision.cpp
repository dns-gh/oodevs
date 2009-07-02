// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_AutomateDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_AutomateDecision.h"

#include "MIL_AutomateType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"
#include "CheckPoints/DIA_Serializer.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

BOOST_CLASS_EXPORT_GUID( DEC_AutomateDecision, "DEC_AutomateDecision" )

//-----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_AutomateDecision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Automate" );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision( MIL_Automate& automate )
    : DEC_Decision             ( automate, "T_Automate" )
    , diaFunctionCaller_       ( automate, automate.GetType().GetFunctionTable() )
    , nForceRatioState_        ( eForceRatioStateNone  )
    , nRulesOfEngagementState_ ( eRoeStateNone         )
    , nCloseCombatState_       ( eCloseCombatStateNone )
    , nOperationalState_       ( eOpStateOperational   )
    , bStateHasChanged_        ( true                  )
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    const DEC_Model_ABC& model = automate.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        DIA_Workspace::Instance().SetObjectName( *this, automate.GetName() ); // ????
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    missionMrtBehaviorParameters_.SetOwnerShip( true );
    missionMrtBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionMrtBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    missionConduiteBehaviorParameters_.SetOwnerShip( true );
    missionConduiteBehaviorParameters_.AddParam( new DIA_Variable_Object () );
    missionConduiteBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision()
    : DEC_Decision             ( "T_Automate" ) 
    , diaFunctionCaller_       ( *(MIL_Automate*)0, *(DIA_FunctionTable< MIL_Automate >*)1 ) // $$$$ JVT : Eurkkk
    , nForceRatioState_        ( eForceRatioStateNone  )
    , nRulesOfEngagementState_ ( eRoeStateNone         )
    , nCloseCombatState_       ( eCloseCombatStateNone )
    , nOperationalState_       ( eOpStateOperational   )
    , bStateHasChanged_        ( true                  )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_AutomateDecision::~DEC_AutomateDecision()
{
    StopDefaultBehavior();

    MT_DELETEOWNED( missionMrtBehaviorParameters_.GetParameters() );
    missionMrtBehaviorParameters_.GetParameters().clear();

    MT_DELETEOWNED( missionConduiteBehaviorParameters_.GetParameters() );
    missionConduiteBehaviorParameters_.GetParameters().clear();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MT_Role_ABC >( *this )
         >> pEntity_
         >> nForceRatioState_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_;
    
    assert( pEntity_ );
    
    uint nID;
    file >> nID;
    
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( nID );
    assert( pType );
    diaFunctionCaller_.DIA_FunctionCaller< MIL_Automate >::DIA_FunctionCaller( *pEntity_, pType->GetFunctionTable() );

    RegisterUserFunctionCaller( diaFunctionCaller_ );
    
    const DEC_Model_ABC& model = pType->GetModel();
    
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        
        DIA_Workspace::Instance().SetObjectName( *this, pEntity_->GetName() ); // ????

        DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
        file >> diaSerializer;
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }
    
    missionMrtBehaviorParameters_.SetOwnerShip( true );
    missionMrtBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionMrtBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    missionConduiteBehaviorParameters_.SetOwnerShip( true );
    missionConduiteBehaviorParameters_.AddParam( new DIA_Variable_Object () );
    missionConduiteBehaviorParameters_.AddParam( new DIA_Variable_Id() );

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pEntity_ );
    unsigned id = pEntity_->GetType().GetID();
    file << boost::serialization::base_object< MT_Role_ABC >( *this )
         << pEntity_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << id;

    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    file << diaSerializer;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::EndCleanStateAfterCrash()
{
}

// =============================================================================
// UPDATE
// =============================================================================

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionMrtBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    ActivateOrder( strBehavior, missionMrtBehaviorParameters_, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionMrtBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    StopMission( strBehavior, missionMrtBehaviorParameters_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionConduiteBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    ActivateOrder( strBehavior, missionConduiteBehaviorParameters_, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionConduiteBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    StopMission( strBehavior, missionConduiteBehaviorParameters_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendFullState( NET_ASN_MsgAutomatAttributes& msg ) const
{
    msg().m.rapport_de_forcePresent    = 1;
    msg().m.roePresent                 = 1;
    msg().m.combat_de_rencontrePresent = 1;
    msg().m.etat_operationnelPresent   = 1;

    msg().rapport_de_force      = (ASN1T_EnumForceRatioStatus)nForceRatioState_;
    msg().combat_de_rencontre   = (ASN1T_EnumMeetingEngagementStatus)nCloseCombatState_;
    msg().etat_operationnel     = (ASN1T_EnumOperationalStatus)nOperationalState_;
    msg().roe                   = (ASN1T_EnumRoe)nRulesOfEngagementState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendChangedState( NET_ASN_MsgAutomatAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetDecAutomate
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_AutomateDecision::GetDecAutomate() const
{
    assert( false );
    throw std::runtime_error( "Unexpected call to DEC_GetAutomate on automate" );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetName
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetName() const
{
    assert( false );
    throw std::runtime_error( "Unexpected call to DEC_GetSzName on automate" );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::HasStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyForceRatioStateChanged( E_ForceRatioState nState )
{
    if( nForceRatioState_ != nState )
    {
        nForceRatioState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState )
{
    if( nRulesOfEngagementState_ != nState )
    {
        nRulesOfEngagementState_ = nState;
        bStateHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyOperationalStateChanged( E_OperationalState nState )
{
    if( nOperationalState_ != nState )
    {
        nOperationalState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetAutomate
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Automate& DEC_AutomateDecision::GetAutomate() const
{
    assert( pEntity_ );
    return *pEntity_;
}
