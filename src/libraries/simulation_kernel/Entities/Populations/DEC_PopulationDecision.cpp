// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_PopulationDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_PopulationDecision.h"

#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "Decision/DEC_ModelPopulation.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Orders/MIL_PopulationMission.h"
#include "Entities/Orders/MIL_Report.h"
#include "CheckPoints/DIA_Serializer.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "Network/NET_ASN_Messages.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

int  DEC_PopulationDecision::nDIAMissionIdx_          = 0;
int  DEC_PopulationDecision::nDIANameIdx_             = 0;
uint DEC_PopulationDecision::nMissionBehaviorDummyId_ = 0;
uint DEC_PopulationDecision::nDefaultBehaviorDummyId_ = 0;

BOOST_CLASS_EXPORT_GUID( DEC_PopulationDecision, "DEC_PopulationDecision" )

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_PopulationDecision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Population" );
    nDIAMissionIdx_ = DEC_Tools::InitializeDIAField( "mission_", diaType );
    nDIANameIdx_    = DEC_Tools::InitializeDIAField( "szName_" , diaType );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population )
    : DEC_Decision_ABC         ( population ) 
    , DIA_Engine               ( *DIA_TypeManager::Instance().GetType( "T_Population" ), "" )
    , pPopulation_             ( &population )
    , diaFunctionCaller_       ( population, population.GetType().GetFunctionTable() )
    , rDominationState_        ( 0. )
    , rLastDominationState_    ( 0. )
    , bStateHasChanged_        ( true )
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    const DEC_ModelPopulation& model = population.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        GetVariable( nDIANameIdx_    ).SetValue( population.GetName() );
        GetVariable( nDIAMissionIdx_ ).Reset();
        DIA_Workspace::Instance().SetObjectName( *this , population.GetName() ); // ????
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    missionBehaviorParameters_.SetOwnerShip( true );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Id    () );

    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision()
    : DEC_Decision_ABC         ( ) 
    , DIA_Engine               ( *DIA_TypeManager::Instance().GetType( "T_Population" ), "" )
    , pPopulation_             ( 0 )
    , diaFunctionCaller_       ( *(MIL_Population*)0, *(DIA_FunctionTable< MIL_Population >*)1 ) // $$$$ JVT : Eurkkk
    , rDominationState_        ( 0. )
    , rLastDominationState_    ( 0. )
    , bStateHasChanged_        ( true )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_PopulationDecision::~DEC_PopulationDecision()
{
    StopDefaultBehavior();
    MT_DELETEOWNED( defaultBehaviorParameters_.GetParameters() );

    MT_DELETEOWNED( missionBehaviorParameters_.GetParameters() );
    missionBehaviorParameters_.GetParameters().clear();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pPopulation_
         >> rDominationState_
         >> rLastDominationState_;
   
    const DEC_ModelPopulation& model = pPopulation_->GetType().GetModel();
    
    diaFunctionCaller_.DIA_FunctionCaller< MIL_Population >::DIA_FunctionCaller( *pPopulation_, pPopulation_->GetType().GetFunctionTable() );
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        GetVariable( nDIANameIdx_    ).SetValue( pPopulation_->GetName() );
        GetVariable( nDIAMissionIdx_ ).Reset();
        DIA_Workspace::Instance().SetObjectName( *this , pPopulation_->GetName() ); // ????

        DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
        file >> diaSerializer;
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    missionBehaviorParameters_.SetOwnerShip( true );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Id    () );

    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pPopulation_ );
    
    file << pPopulation_
         << rDominationState_
         << rLastDominationState_;

    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    file << diaSerializer;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::CleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::CleanStateAfterCrash()
{
    assert( false ); // To allow debugging ...
    assert( pPopulation_ );   
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

namespace
{
    void LogCrash( MIL_Population* pPopulation_ )
    {
        MT_LOG_ERROR_MSG( "Population " << pPopulation_->GetID() << " ('" << pPopulation_->GetName() << "') : Mission '" << pPopulation_->GetOrderManager().GetMissionName() << "' impossible" );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::UpdateDecision
// Last modified: JVT 02-12-16
//-----------------------------------------------------------------------------
void DEC_PopulationDecision::UpdateDecision()
{
    __try
    {
        PrepareUpdate    ();
        UpdateMotivations( (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
        UpdateDecisions  ();
        ExecuteAllActions();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        assert( pPopulation_ );
        LogCrash( pPopulation_ );
        CleanStateAfterCrash();
        MIL_Report::PostEvent( *pPopulation_, MIL_Report::eReport_MissionImpossible_ );
        pPopulation_->GetOrderManager().ReplaceMission();
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( MIL_PopulationMission& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    missionBehaviorParameters_.GetParameter( 0 ).SetValue( mission );
    missionBehaviorParameters_.GetParameter( 1 ).SetValue( (int)nMissionBehaviorDummyId_++ );
    DIA_ActivateOrder( &GetBehaviorPart(), strBehavior, 1.0, missionBehaviorParameters_ );
    GetVariable( nDIAMissionIdx_ ).SetValue( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( MIL_PopulationMission& mission )
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

// =============================================================================
// DEFAULT BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::IsDefaultBehaviorAvailable
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
bool DEC_PopulationDecision::IsDefaultBehaviorAvailable() const
{
    return GetBehaviorPart().FindBehavior( "BEH_Defaut" ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartDefaultBehavior
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
    {
        defaultBehaviorParameters_.GetParameter( 0 ).SetValue( (int)nDefaultBehaviorDummyId_++ ); 
        DIA_ActivateOrder( &GetBehaviorPart(), "BEH_Defaut", 1.0, defaultBehaviorParameters_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopDefaultBehavior
// Created: JVT 2004-12-14
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
        DIA_DesactivateOrder( &GetBehaviorPart(), "BEH_Defaut", defaultBehaviorParameters_, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Reset
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Reset()
{
    StopDefaultBehavior ();
    StartDefaultBehavior();
    assert( pMotivationTool_ );
    static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ).Reset();
} 

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendFullState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendFullState( NET_ASN_MsgPopulationUpdate& msg )
{
    msg().m.etat_dominationPresent = 1;
    msg().etat_domination          = (uint)( rDominationState_ * 100. );
    rLastDominationState_ = rDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendChangedState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendChangedState( NET_ASN_MsgPopulationUpdate& msg )
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
MT_Float DEC_PopulationDecision::GetDominationState() const
{
    return rDominationState_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetBehaviorPart
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
DIA_BehaviorPart& DEC_PopulationDecision::GetBehaviorPart() const
{
    assert( pBehaviorTool_ != 0 );
    return( static_cast< DIA_BehaviorPart& >( *pBehaviorTool_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetPopulation
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Population& DEC_PopulationDecision::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::NotifyDominationStateChanged( MT_Float rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rDominationState_ == rValue )
        return;

    rDominationState_ = rValue;

    static const MT_Float rDeltaPercentageForNetwork = 0.05;
    if( fabs( rLastDominationState_ - rDominationState_ ) > rDeltaPercentageForNetwork || rDominationState_ == 0. || rDominationState_ == 1. )
        bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Clean
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::HasStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
bool DEC_PopulationDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}


