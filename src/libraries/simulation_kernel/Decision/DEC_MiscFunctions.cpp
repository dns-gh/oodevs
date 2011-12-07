// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_MiscFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_MiscFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_MiscFunctions.h"

#include "Decision/DEC_Representations.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

std::set<  boost::shared_ptr< DEC_Knowledge_Agent > > DEC_MiscFunctions::enemyRepresentations_;

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetCurrentSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetCurrentSpeedModificator( MIL_AgentPion& callerAgent, double rFactor )
{
    callerAgent.GetRole< moving::PHY_RoleAction_Moving >().SetSpeedModificator( rFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::SetMaxSpeedModificator( MIL_AgentPion& callerAgent, double rFactor )
{
    callerAgent.GetRole< moving::PHY_RoleAction_Moving >().SetMaxSpeedModificator( rFactor );
}


// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetMaxSpeedModificator
// Created: LMT 2011-07-06
// -----------------------------------------------------------------------------
double DEC_MiscFunctions::GetMaxSpeedModificator( const DEC_Decision_ABC* agent )
{
    if( !agent )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return agent->GetPion().GetRole< moving::PHY_RoleAction_Moving >().GetMaxSpeedModificator();
}

// =============================================================================
// REINFORCEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetAgentReinforcements
// Created: MGD 2010-08-27
// -----------------------------------------------------------------------------
std::vector<DEC_Decision_ABC*> DEC_MiscFunctions::GetAgentReinforcements( const DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
   return DEC_MiscFunctions::GetReinforcements( pAgent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetReinforcements
// Created: NLD 2004-10-01
// Modified: RPD 2009-08-03
// -----------------------------------------------------------------------------
std::vector<DEC_Decision_ABC*> DEC_MiscFunctions::GetReinforcements( const MIL_AgentPion& callerAgent )
{
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    std::vector<DEC_Decision_ABC*> result;
    result.reserve( reinforcements.size() );
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        result.push_back( &(**it).GetRole< DEC_RolePion_Decision >() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetAgentReinforcementsNumber
// Created: LDC 2011-08-30
// -----------------------------------------------------------------------------
int DEC_MiscFunctions::GetAgentReinforcementsNumber( const DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    const MIL_AgentPion& pion = pAgent->GetPion();
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    return (int) reinforcements.size();
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::Reinforce
// Created: NLD 2003-03-12
// Modified: RPD 2009-08-03
//-----------------------------------------------------------------------------
bool DEC_MiscFunctions::Reinforce( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget )
{
    if ( !pTarget )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().Reinforce( pTarget->GetPion() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::CancelReinforcement
// Created: NLD 2003-03-12
// Modified: RPD 2009-08-03
//-----------------------------------------------------------------------------
void DEC_MiscFunctions::CancelReinforcement( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Reinforcement >().CancelReinforcement();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MIL_FragOrder > > DEC_MiscFunctions::GetOrdersCategory( MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    return role.GetOrdersCategory();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_PathPoint > > DEC_MiscFunctions::GetPointsCategory( MIL_Entity_ABC& callerAgent )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    return role.GetPointsCategory();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::RemoveFromOrdersCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveFromOrdersCategory( MIL_Entity_ABC& callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.RemoveFromOrdersCategory( pOrder );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::RemoveFromPointsCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveFromPointsCategory( MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_PathPoint > pPoint )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.RemoveFromPointsCategory( pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DeleteOrderRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::DeleteOrderRepresentation( MIL_Entity_ABC& callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder )
{
    DEC_Representations& role = callerAgent.GetRole< DEC_Representations >();
    role.DeleteOrderRepresentation( pOrder );
}

namespace DEC_DecisionImpl
{
    void RegisterMissionParameters( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::FillMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::FillMissionParameters( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& initTaskFunction, const directia::tools::binders::ScriptRef& refMission, boost::shared_ptr< MIL_Mission_ABC > pMission, bool isMasalife )
{
    if( pMission )
        DEC_DecisionImpl::RegisterMissionParameters( brain, initTaskFunction, refMission, pMission, isMasalife );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_MiscFunctions::GetName( DEC_Decision_ABC* pEntity )
{
    if( !pEntity )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return pEntity->GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetAutomate
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_MiscFunctions::GetAutomate( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return pAgent->GetDecAutomate();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetDirectionEnnemi
// Created: LDC 2009-04-20
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_MiscFunctions::GetDirectionEnnemi( boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    boost::shared_ptr< MT_Vector2D > result( new MT_Vector2D( pMission->GetDirDanger() ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::CopyDirectionDanger
// Created: LDC 2009-04-21
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::CopyDirectionDanger( MT_Vector2D* pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pPosSource || pPosSource->IsZero() || !( MT_IsZero( pPosSource->SquareMagnitude() - 1. ) ) )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    pMission->AffectDirection( *pPosSource );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetPointXY
// Created: PSN & GGE 2010-03-25
// -----------------------------------------------------------------------------
std::string DEC_MiscFunctions::GetPointXY( boost::shared_ptr< MT_Vector2D > point )
{
    if( !point )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    std::stringstream stream;
    stream << "x = " << point->rX_ << ", y = " << point->rY_;
    return stream.str();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::GetTimeInSeconds
// Created: BCI 2011-02-24
// -----------------------------------------------------------------------------
unsigned int DEC_MiscFunctions::GetTimeInSeconds()
{
    return MIL_AgentServer::GetWorkspace().GetRealTime();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::AddEnemyRepresentation
// Created: LDC 2011-09-12
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::AddEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent )
{
    enemyRepresentations_.insert( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::AddEnemyRepresentation
// Created: LDC 2011-09-12
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::RemoveEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent )
{
    enemyRepresentations_.erase( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::AddEnemyRepresentation
// Created: LDC 2011-09-12
// -----------------------------------------------------------------------------
std::vector<  boost::shared_ptr< DEC_Knowledge_Agent > > DEC_MiscFunctions::GetEnemyRepresentation( DEC_Decision_ABC* caller )
{
    if( !caller )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    std::vector<  boost::shared_ptr< DEC_Knowledge_Agent > > result;
    MIL_Army_ABC& army = caller->GetPion().GetArmy();
    for ( std::set<  boost::shared_ptr< DEC_Knowledge_Agent > >::const_iterator it = enemyRepresentations_.begin(); it != enemyRepresentations_.end(); ++it )
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        if( (*it)->IsValid() && (*it)->IsAnEnemy( army ) )
            result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::Report
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::Report( DEC_Decision_ABC& caller, int type, int reportId )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportAgentKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportAgentKnowledge( DEC_Decision_ABC& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::CreateAgentKnowledge( agentKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportDotationType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportDotationType( DEC_Decision_ABC& caller, int type, int reportId, const PHY_DotationCategory* dotationType )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::CreateDotationType( dotationType ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportEquipmentType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportEquipmentType( DEC_Decision_ABC& caller, int type, int reportId, const PHY_ComposanteTypePion* equipmentType )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::CreateEquipmentType( equipmentType ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportFloat
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportFloat( DEC_Decision_ABC& caller, int type, int reportId, float param )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( param ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportFloatFloat
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportFloatFloat( DEC_Decision_ABC& caller, int type, int reportId, float param1, float param2 )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam1( MIL_MissionParameterFactory::Create( param1 ) );
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam2( MIL_MissionParameterFactory::Create( param2 ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportId
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportId( DEC_Decision_ABC& caller, int type, int reportId, int id )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( id ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportObjectKnoweldge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportObjectKnoweldge( DEC_Decision_ABC& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::CreateObjectKnowledge( pKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportPion
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportPion( DEC_Decision_ABC& caller, int type, int reportId, DEC_Decision_ABC* pion )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        DEC_RolePion_Decision* pionDec = static_cast< DEC_RolePion_Decision* >( pion ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::CreatePion( pionDec ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportPionAutomate
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportPionAutomate( DEC_Decision_ABC& caller, int type, int reportId, DEC_Decision_ABC* pion, DEC_Decision_ABC* automate )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam1( MIL_MissionParameterFactory::CreatePion( pion ) );
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam2( MIL_MissionParameterFactory::CreateAutomat( automate ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportPionPion
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportPionPion( DEC_Decision_ABC& caller, int type, int reportId, DEC_Decision_ABC* pion1, DEC_Decision_ABC* pion2 )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        DEC_RolePion_Decision* pionDec = static_cast< DEC_RolePion_Decision* >( pion1 ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam1( MIL_MissionParameterFactory::CreatePion( pionDec ) );
        DEC_RolePion_Decision* pionDec2 = static_cast< DEC_RolePion_Decision* >( pion2 ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam2( MIL_MissionParameterFactory::CreateAutomat( pionDec2 ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}


// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportPopulationKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportPopulationKnowledge( DEC_Decision_ABC& caller, int type, int reportId, int populationKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(populationKnowledge);
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam( MIL_MissionParameterFactory::CreatePopulationKnowledge( pKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportTirPion
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportTirPion( DEC_Decision_ABC& caller, int type, int reportId, int id )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam( MIL_MissionParameterFactory::CreateTir( id ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: static void DEC_MiscFunctions::ReportString
// Created: MGD 2010-03-24
// -----------------------------------------------------------------------------
void DEC_MiscFunctions::ReportString( DEC_Decision_ABC& caller, int type, int reportId, const std::string& message )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( message ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}
