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

#include "MIL_pch.h"

#include "DEC_PopulationDecision.h"

#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "Decision/DEC_ModelPopulation.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "Entities/Orders/Population/MIL_PopulationMission_ABC.h"
#include "Entities/RC/MIL_RC.h"

#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"


int  DEC_PopulationDecision::nDIAMissionIdx_          = 0;
int  DEC_PopulationDecision::nDIANameIdx_             = 0;
uint DEC_PopulationDecision::nMissionBehaviorDummyId_ = 0;
uint DEC_PopulationDecision::nDefaultBehaviorDummyId_ = 0;

//BOOST_CLASS_EXPORT_GUID( DEC_PopulationDecision, "DEC_PopulationDecision" )

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
    : DIA_Engine               ( *DIA_TypeManager::Instance().GetType( "T_Population" ), "" )
    , pPopulation_             ( &population )
    , diaFunctionCaller_       ( population, population.GetType().GetFunctionTable() )
{
    RegisterUserFunctionCaller( diaFunctionCaller_ );

    const DEC_ModelPopulation& model = population.GetType().GetModel();
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        GetVariable( nDIANameIdx_    ).SetValue( population.GetName() );
        GetVariable( nDIAMissionIdx_ ).SetValue( *(DIA_TypedObject*)0 );
        DIA_Workspace::Instance().SetObjectName( *this, population.GetName() ); // ????
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
    : DIA_Engine               ( *DIA_TypeManager::Instance().GetType( "T_Population" ), "" )
    , pPopulation_             ( 0 )
    , diaFunctionCaller_       ( *(MIL_Population*)0, *(DIA_FunctionTable< MIL_Population >*)1 ) // $$$$ JVT : Eurkkk
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
/*
// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pPopulation_
         >> nForceRatioState_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_;
    
    assert( pPopulation_ );
    
    uint nID;
    file >> nID;
    
    const MIL_PopulationType* pType = MIL_PopulationType::FindPopulationType( nID );
    assert( pType );
    diaFunctionCaller_.DIA_FunctionCaller< MIL_Population >::DIA_FunctionCaller( *pPopulation_, pType->GetFunctionTable() );

    RegisterUserFunctionCaller( diaFunctionCaller_ );
    
    const DEC_ModelPopulation& model = pType->GetModel();
    
    try
    {
        SetType ( model.GetDIAType() );
        CopyFrom( &model.GetDIAModel() );
        
        GetVariable( nDIAMissionIdx_ ).SetValue( *(DIA_TypedObject*)0 );
        DIA_Workspace::Instance().SetObjectName( *this, pPopulation_->GetName() ); // ????

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
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pPopulation_ );
    
    file << pPopulation_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << pPopulation_->GetType().GetID();

    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    file << diaSerializer;
}
*/

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

    GetBehaviorPart().ResetPart();
    Reset();
    while( GetContext().GetLocation() != 0 )
        GetContext().ExitContext();
}

// =============================================================================
// UPDATE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::UpdateDecision
// Last modified: JVT 02-12-16
//-----------------------------------------------------------------------------
void DEC_PopulationDecision::UpdateDecision()
{
    try
    {
        PrepareUpdate    ();
        UpdateMotivations( (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
        UpdateDecisions  ();
        ExecuteAllActions();
    }
#ifdef _DEBUG
    catch( DIA_Script_Exception& e )
#else
    catch( ... )
#endif    
    {
        assert( pPopulation_ );
        MT_LOG_ERROR_MSG( MT_FormatString( "Population %d ('%s') : Mission '%s' impossible", pPopulation_->GetID(), pPopulation_->GetName().c_str(), pPopulation_->GetOrderManager().GetMission() ? pPopulation_->GetOrderManager().GetMission()->GetName() : "Default" ) );
        MIL_RC::pRcMissionImpossible_->Send( *pPopulation_, MIL_RC::eRcTypeMessage );
        pPopulation_->GetOrderManager().CancelAllOrders();       
        CleanStateAfterCrash();
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( MIL_PopulationMission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetBehaviorName();
    missionBehaviorParameters_.GetParameter( 0 ).SetValue( mission );
    missionBehaviorParameters_.GetParameter( 1 ).SetValue( (int)nMissionBehaviorDummyId_++ );
    DIA_ActivateOrder( &GetBehaviorPart(), strBehavior, 1.0, missionBehaviorParameters_ );
    GetVariable( nDIAMissionIdx_ ).SetValue( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( MIL_PopulationMission_ABC& mission )
{
    try
    {
        const std::string& strBehavior = mission.GetType().GetBehaviorName();
        DIA_DesactivateOrder( &GetBehaviorPart(), strBehavior, missionBehaviorParameters_, true );
        GetVariable( nDIAMissionIdx_ ).SetValue( *(MIL_PopulationMission_ABC*)0 );
    }
    catch( ... )
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
