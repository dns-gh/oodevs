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

#include "MIL_PopulationType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "CheckPoints/DIA_Serializer.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "Network/NET_ASN_Messages.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

BOOST_CLASS_EXPORT_GUID( DEC_PopulationDecision, "DEC_PopulationDecision" )

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::InitializeDIA
// Created: AGN 03-03-28
//-----------------------------------------------------------------------------
// static
void DEC_PopulationDecision::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Population" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population )
    : DEC_Decision             ( population, "T_Population" )
    , diaFunctionCaller_       ( population, population.GetType().GetFunctionTable() )
    , rDominationState_        ( 0. )
    , rLastDominationState_    ( 0. )
    , bStateHasChanged_        ( true )
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    const DEC_Model_ABC& model = population.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        name_ = population.GetName();
        DIA_Workspace::Instance().SetObjectName( *this , population.GetName() ); // ????
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    missionBehaviorParameters_.SetOwnerShip( true );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Object() );
    missionBehaviorParameters_.AddParam( new DIA_Variable_Id    () );

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision()
    : DEC_Decision             ( "T_Population" ) 
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
    file >> boost::serialization::base_object< MT_Role_ABC >( *this )
         >> pEntity_
         >> rDominationState_
         >> rLastDominationState_;
   
    const DEC_Model_ABC& model = pEntity_->GetType().GetModel();
    
    diaFunctionCaller_.DIA_FunctionCaller< MIL_Population >::DIA_FunctionCaller( *pEntity_, pEntity_->GetType().GetFunctionTable() );
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        name_ = pEntity_->GetName();
        DIA_Workspace::Instance().SetObjectName( *this , pEntity_->GetName() ); // ????

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

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pEntity_ );
    
    file << boost::serialization::base_object< MT_Role_ABC >( *this )
         << pEntity_
         << rDominationState_
         << rLastDominationState_;

    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    file << diaSerializer;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::EndCleanStateAfterCrash()
{
}

// =============================================================================
// UPDATE
// =============================================================================

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();    
    ActivateOrder( strBehavior, missionBehaviorParameters_, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    StopMission( strBehavior, missionBehaviorParameters_ );
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

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetPopulation
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Population& DEC_PopulationDecision::GetPopulation() const
{
    assert( pEntity_ );
    return *pEntity_;
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

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDecAutomate
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_PopulationDecision::GetDecAutomate() const
{
    assert( false );
    throw std::runtime_error( "DEC_GetAutomate unexpected on population" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_PopulationDecision::GetName() const
{
    return name_;
}

