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

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_ModelPion.h"
#include "Decision/DEC_Tools.h"
#include "CheckPoints/DIA_Serializer.h"
#include "Network/NET_ASN_Messages.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

int  DEC_RolePion_Decision::nDIAMissionIdx_          = 0; 
int  DEC_RolePion_Decision::nDIANameIdx_             = 0;
int  DEC_RolePion_Decision::nDIAAutomateIdx_         = 0;
uint DEC_RolePion_Decision::nDefaultBehaviorDummyId_ = 0;
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
    : MT_Role_ABC               ( role )
    , pPion_                    ( &pion )
    , DIA_Engine                ( *DIA_TypeManager::Instance().GetType( "T_Pion" ), "" )
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
    
    const DEC_ModelPion& model = pion.GetType().GetModel();
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
    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
    StartDefaultBehavior();    
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::DEC_RolePion_Decision()
    : pPion_                    ( 0 )
    , DIA_Engine                ( *DIA_TypeManager::Instance().GetType( "T_Pion" ), "" )
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
    MT_DELETEOWNED( defaultBehaviorParameters_.GetParameters() );
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
         >> pPion_
         >> nForceRatioState_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_
         >> nIndirectFireAvailability_;
    assert( pPion_ );

    uint nRoePopulationID;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );
    assert( pRoePopulation_ );
       
    uint nPionTypeID;
    file >> nPionTypeID;
          
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nPionTypeID );
    assert( pType );    

    diaFunctionCaller_.DIA_FunctionCaller< MIL_AgentPion >::DIA_FunctionCaller( *pPion_, pType->GetFunctionTable() );

    RegisterUserFunctionCaller( diaFunctionCaller_ );
 
    const DEC_ModelPion& model = pType->GetModel();
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
        
        DIA_Workspace::Instance().SetObjectName( *this, pPion_->GetName() ); // ????
        
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
    defaultBehaviorParameters_.SetOwnerShip( true );
    defaultBehaviorParameters_.AddParam( new DIA_Variable_Id() );
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    DIA_Serializer diaSerializer( static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ) );
    
    file << boost::serialization::base_object< MT_Role_ABC >( *this )
         << pPion_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << nIndirectFireAvailability_
         << pRoePopulation_->GetID()
         << pPion_->GetType().GetID()
         << const_cast< DEC_RolePion_Decision& >( *this ).GetVariable( nDIANameIdx_     ).ToString()  // $$$$ JVT : Beark
         << static_cast< DEC_AutomateDecision* >( const_cast< DEC_RolePion_Decision& >( *this ).GetVariable( nDIAAutomateIdx_ ).ToObject() ) // $$$$ JVT : Beark Arrrg
         << diaSerializer;         
         
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::CleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::CleanStateAfterCrash()
{
    assert( false ); // To allow debugging ...        
    assert( pPion_ );    
    _clearfp();

    DEC_Tools::DisplayDiaStack( GetCurrentInstance(), GetCurrentDebugInfo() );

    while( GetContext().GetLocation() != 0 )
        GetContext().ExitContext();

    GetBehaviorPart().ResetPart();
    Reset();

    pPion_->CancelAllActions();
    GetRole< PHY_RolePion_Perceiver >().DisableAllPerceptions();
}


// =============================================================================
// DEFAULT BEHAVIOR MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsDefaultBehaviorAvailable
// Created: NLD 2004-01-16
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsDefaultBehaviorAvailable() const
{
    return GetBehaviorPart().FindBehavior( "BEH_Defaut" ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StartDefaultBehavior
// Created: NLD 2004-01-16
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StartDefaultBehavior()
{                                 
    if( !IsDefaultBehaviorAvailable() )
        return;
    defaultBehaviorParameters_.GetParameter( 0 ).SetValue( (int)nDefaultBehaviorDummyId_++ ); 
    DIA_ActivateOrder( &GetBehaviorPart(), "BEH_Defaut", 1.0, defaultBehaviorParameters_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopDefaultBehavior
// Created: NLD 2004-01-16
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StopDefaultBehavior()
{
    if( !IsDefaultBehaviorAvailable() )
        return;
    DIA_DesactivateOrder( &GetBehaviorPart(), "BEH_Defaut", defaultBehaviorParameters_, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::Reset
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::Reset()
{
    StopDefaultBehavior ();
    StartDefaultBehavior();
    assert( pMotivationTool_ );
    static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ).Reset();
} 

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
    assert( pPion_ );
    GetVariable( nDIAAutomateIdx_ ).SetValue( pPion_->GetAutomate().GetDecision() );
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

namespace
{
    void LogCrash( MIL_AgentPion& pion )
    {
        MT_LOG_ERROR_MSG( "Pion " << pion.GetID() << "('" << pion.GetName() << "') : Mission '" << pion.GetOrderManager().GetMissionName() << "' impossible" );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::UpdateDecision
// Last modified: JVT 02-12-16
//-----------------------------------------------------------------------------
void DEC_RolePion_Decision::UpdateDecision()
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
        assert( pPion_ );
        LogCrash( *pPion_ );
        CleanStateAfterCrash();       
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_MissionImpossible_ );
        pPion_->GetOrderManager().ReplaceMission();       
    }
}

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

    msg().rapport_de_force              = (ASN1T_EnumEtatRapFor)nForceRatioState_;
    msg().combat_de_rencontre           = (ASN1T_EnumEtatCombatRencontre)nCloseCombatState_;
    msg().etat_operationnel             = (ASN1T_EnumEtatOperationnel)nOperationalState_;
    msg().disponibilite_au_tir_indirect = (ASN1T_EnumDisponibiliteAuTir)nIndirectFireAvailability_;
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

