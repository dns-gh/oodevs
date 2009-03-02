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
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"
#include "CheckPoints/DIA_Serializer.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

int  DEC_AutomateDecision::nDIAMissionIdx_                  = 0;
uint DEC_AutomateDecision::nMissionMrtBehaviorDummyId_      = 0;
uint DEC_AutomateDecision::nMissionConduiteBehaviorDummyId_ = 0;
uint DEC_AutomateDecision::nDefaultBehaviorDummyId_         = 0;

BOOST_CLASS_EXPORT_GUID( DEC_AutomateDecision, "DEC_AutomateDecision" )

//-----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_AutomateDecision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Automate" );
    nDIAMissionIdx_   = DEC_Tools::InitializeDIAField( "mission_"  , diaType );
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
        GetVariable( nDIAMissionIdx_ ).Reset();
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

    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
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
    MT_DELETEOWNED( defaultBehaviorParameters_.GetParameters() );

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
        
        GetVariable( nDIAMissionIdx_ ).Reset();
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

    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
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
// Name: DEC_AutomateDecision::CleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::CleanStateAfterCrash()
{
    assert( false ); // To allow debugging ...
    assert( pEntity_ );   
    _clearfp();

    DEC_Tools::DisplayDiaStack( GetCurrentInstance(), GetCurrentDebugInfo() );
    
    GetBehaviorPart().ResetPart();
    Reset();
    while( GetContext().GetLocation() != 0 )
        GetContext().ExitContext();
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
void DEC_AutomateDecision::StartMissionMrtBehavior( MIL_AutomateMission& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    missionMrtBehaviorParameters_.GetParameter( 0 ).SetValue( mission );
    missionMrtBehaviorParameters_.GetParameter( 1 ).SetValue( (int)nMissionMrtBehaviorDummyId_++ );
    DIA_ActivateOrder( &GetBehaviorPart(), strBehavior, 1.0, missionMrtBehaviorParameters_ );
    GetVariable( nDIAMissionIdx_ ).SetValue( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionMrtBehavior( MIL_AutomateMission& mission )
{
    __try
    {
        const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
        DIA_DesactivateOrder( &GetBehaviorPart(), strBehavior, missionMrtBehaviorParameters_, true );
        GetVariable( nDIAMissionIdx_ ).Reset();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        CleanStateAfterCrash();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionConduiteBehavior( MIL_AutomateMission& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    missionConduiteBehaviorParameters_.GetParameter( 0 ).SetValue( mission );
    missionConduiteBehaviorParameters_.GetParameter( 1 ).SetValue( (int)nMissionConduiteBehaviorDummyId_++ );
    DIA_ActivateOrder( &GetBehaviorPart(), strBehavior, 1.0, missionConduiteBehaviorParameters_ );
    GetVariable( nDIAMissionIdx_ ).SetValue( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionConduiteBehavior( MIL_AutomateMission& mission )
{
    __try
    {
        const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
        DIA_DesactivateOrder( &GetBehaviorPart(), strBehavior, missionConduiteBehaviorParameters_, true );
        GetVariable( nDIAMissionIdx_ ).Reset();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        CleanStateAfterCrash();
    }
}

// =============================================================================
// DEFAULT BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::IsDefaultBehaviorAvailable
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::IsDefaultBehaviorAvailable() const
{
    return GetBehaviorPart().FindBehavior( "BEH_Defaut" ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartDefaultBehavior
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
    {
        defaultBehaviorParameters_.GetParameter( 0 ).SetValue( (int)nDefaultBehaviorDummyId_++ ); 
        DIA_ActivateOrder( &GetBehaviorPart(), "BEH_Defaut", 1.0, defaultBehaviorParameters_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopDefaultBehavior
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
        DIA_DesactivateOrder( &GetBehaviorPart(), "BEH_Defaut", defaultBehaviorParameters_, true );
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
