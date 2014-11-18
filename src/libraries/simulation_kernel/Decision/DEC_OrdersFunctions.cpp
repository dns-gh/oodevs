// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.cpp $
// $Author: Nld $
// $Modtime: 2/12/04 10:15 $
// $Revision: 4 $
// $Workfile: DEC_OrdersFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_OrdersFunctions.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_LimaOrder.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"

void DEC_OrdersFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_FinMission", &DEC_OrdersFunctions::FinishMission );
    brain.RegisterFunction( "_DEC_NouvelleMission", &DEC_OrdersFunctions::IsNewMissionStarted );
    brain.RegisterFunction( "_DEC_GetLima", &DEC_OrdersFunctions::GetLima );
    brain.RegisterFunction( "_DEC_GetLimasFromType", &DEC_OrdersFunctions::GetLimasFromType );
    brain.RegisterFunction( "_DEC_ProchaineLimaHoraireNonFlagee", &DEC_OrdersFunctions::GetNextScheduledLima );
    brain.RegisterFunction( "_DEC_Fuseau", &DEC_OrdersFunctions::GetFuseau );
    brain.RegisterFunction( "_DEC_SetMissionLimaFlag", &DEC_OrdersFunctions::AutomateSetMissionLimaFlag );
    brain.RegisterFunction( "_DEC_GetMissionLimaFlag", &DEC_OrdersFunctions::GetMissionLimaFlag );
    brain.RegisterFunction( "_DEC_SetMissionLimaFlagHoraire", &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag );

    // MRT / conduite
    brain.RegisterFunction( "_DEC_MRT_CreerMissionPion", &DEC_OrdersFunctions::MRT_CreatePionMission );
    brain.RegisterFunction( "_DEC_MRT_Valide", &DEC_OrdersFunctions::MRT_Validate );
    brain.RegisterFunction( "_DEC_MRT_AffecteFuseaux", &DEC_OrdersFunctions::MRT_AffectFuseaux );
    brain.RegisterFunction( "_DEC_CDT_CreerMissionPion", &DEC_OrdersFunctions::CDT_CreatePionMission );
    brain.RegisterFunction( "_DEC_CDT_DonnerMissionPion_Mission", &DEC_OrdersFunctions::CDT_GivePionMission );
    brain.RegisterFunction( "_DEC_CreerMissionAutomate", &DEC_OrdersFunctions::CreateAutomateMission );
    brain.RegisterFunction( "_DEC_DonnerMissionAutomate_Mission", &DEC_OrdersFunctions::GiveAutomateMission );
    brain.RegisterFunction( "_DEC_DonnerMissionAutomate", &DEC_OrdersFunctions::GiveAutomateMissionToAutomat );
    brain.RegisterFunction( "_DEC_DecouperFuseau", &DEC_OrdersFunctions::SplitFuseau );

    // Agent
    brain.RegisterFunction( "_DEC_CreerMissionPion", &DEC_OrdersFunctions::CreatePionMissionBM );
    brain.RegisterFunction( "_DEC_CreerMissionPionVersPion", &DEC_OrdersFunctions::CreatePionMissionVersPionBM );
    brain.RegisterFunction( "_DEC_DonnerMissionPion", &DEC_OrdersFunctions::CDT_GiveMission );
    brain.RegisterFunction( "_DEC_DonnerMissionPionVersPion", &DEC_OrdersFunctions::CDT_GiveMissionVersPion );

    brain.RegisterFunction( "DEC_IsMissionPionAvailable", &DEC_OrdersFunctions::IsPionMissionAvailable );
    brain.RegisterFunction( "DEC_IsAutomateMissionPionAvailable", &DEC_OrdersFunctions::IsAutomateMissionAvailable );
    brain.RegisterFunction( "DEC_DonnerMissionADAAutomate", &DEC_OrdersFunctions::GiveMissionToAutomat );

    brain.RegisterFunction( "DEC_GetFuseau", &DEC_OrdersFunctions::GetFuseau );

    brain.RegisterFunction( "DEC_IsFragOrderAvailable", &DEC_OrdersFunctions::IsFragOrderAvailable );
    brain.RegisterFunction( "DEC_IsMissionAvailable", &DEC_OrdersFunctions::IsPionMissionAvailable );
    brain.RegisterFunction( "DEC_Mission_IsPath", &DEC_OrdersFunctions::DEC_Mission_IsPath );
    brain.RegisterFunction( "DEC_IsAutomateMissionAvailable", &DEC_OrdersFunctions::IsAutomateMissionAvailable );

    brain.RegisterFunction( "DEC_AssignerFuseauAMissionPion_Mission", &DEC_OrdersFunctions::AssignFuseauToPionMission );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionAutomate_Mission", &DEC_OrdersFunctions::AssignFuseauToAutomateMission );
    brain.RegisterFunction( "DEC_AssignerDirectionAMissionAutomate_Mission", &DEC_OrdersFunctions::AssignDirectionToAutomateMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::FinishMission( DEC_Decision_ABC* caller )
{
    caller->GetOrderManager().CancelMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::IsNewMissionStarted( DEC_Decision_ABC* caller )
{
    return caller->GetOrderManager().IsNewMissionStarted();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLima
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
unsigned int DEC_OrdersFunctions::GetLima( const DEC_Decision_ABC* caller, unsigned int limaId )
{
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( limaId );
    if( !pFunction )
        return 0;
    MIL_LimaOrder* pLima = caller->GetOrderManager().FindLima( *pFunction );
    if( !pLima )
        return 0;
    return pLima->GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLimasFromType
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
std::vector< unsigned int > DEC_OrdersFunctions::GetLimasFromType( const DEC_Decision_ABC* caller, unsigned int limaId )
{
    std::vector< unsigned int > vecIDs;
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( limaId );
    if( !pFunction )
        return vecIDs;
    std::vector< MIL_LimaOrder* > vecLimas = caller->GetOrderManager().FindAllLimas( *pFunction );
    for( auto it = vecLimas.begin(); it != vecLimas.end(); ++it )
        vecIDs.push_back( (*it)->GetID() );
    return vecIDs;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetNextScheduledLima
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
MIL_LimaOrder* DEC_OrdersFunctions::GetNextScheduledLima( const DEC_Decision_ABC* caller )
{
    return caller->GetOrderManager().FindNextScheduledLima();
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::MRT_CreatePionMission( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = callerAutomate->GetAutomate().GetOrderManager().MRT_CreatePionMission( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_Validate( DEC_Decision_ABC* callerAutomate )
{
    callerAutomate->GetAutomate().GetOrderManager().MRT_Validate();
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_AffectFuseaux
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_AffectFuseaux( DEC_Decision_ABC* callerAutomate, std::vector< DEC_Decision_ABC* > pions )
{
    MIL_Automate& automate = callerAutomate->GetAutomate();
    //$$$ NAZE
    assert( automate.IsEngaged() );
    // Découpage
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    if( !automate.GetOrderManager().GetFuseau().SplitIntoSubFuseaux( static_cast< unsigned int >( pions.size() ), subFuseaux ) )
        return;
    // Affectation des fuseaux
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        if( !(*itPion) )
            throw MASA_EXCEPTION( "invalid parameter." );
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        MIL_Fuseau::IT_FuseauPtrList itFuseau;
        for( itFuseau = subFuseaux.begin(); itFuseau != subFuseaux.end(); ++itFuseau )
        {
            MIL_Fuseau* fuseau = *itFuseau;
            if( !fuseau )
                throw MASA_EXCEPTION( "invalid parameter." );
            if( fuseau->IsInside( pion.GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
            {
                automate.GetOrderManager().MRT_SetFuseauForPion( pion, *fuseau );
                break;
            }
        }
        // Pas de fuseau trouvé => prend le 1er
        if( itFuseau == subFuseaux.end() )
        {
            MIL_Fuseau* fuseau = subFuseaux.front();
            if( !fuseau )
                throw MASA_EXCEPTION( "invalid parameter." );
            automate.GetOrderManager().MRT_SetFuseauForPion( pion, *fuseau );
            subFuseaux.erase( subFuseaux.begin() );
        }
        else
            subFuseaux.erase( itFuseau );
    }
    assert( subFuseaux.empty() );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_CreatePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CDT_CreatePionMission( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    if( !pMissionType )
    {
        MT_LOG_ERROR( "Mission '" << mission << "' does not exist", 4, "DEC_OrdersFunctions::CDT_CreatePionMission" );
        throw MASA_EXCEPTION( mission + " does not exist" );
    }
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = callerAutomate->GetAutomate().GetOrderManager().CDT_CreatePionMission( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CreatePionMissionBM
// Created: MGD 2010-01-14
// Like CDT_CreatePionMission but no need of active MRT
//-----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CreatePionMissionBM( DEC_Decision_ABC* pPion, const std::string& mission )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = pPion->GetPion().GetAutomate().GetOrderManager().CreatePionMissionBM( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CreatePionMissionVersPionBM
// Created: GGE 2010-06-11
// Like CDT_CreatePionMission but no need of active MRT
//-----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CreatePionMissionVersPionBM( DEC_Decision_ABC* pPion, const std::string& mission )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = pPion->GetPion().GetAutomate().GetOrderManager().CreatePionMissionVersPionBM( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GivePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GivePionMission( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    callerAutomate->GetAutomate().GetOrderManager().CDT_GivePionMission( pMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GivePionMission
// Created: GGE 2010-06-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GivePionMissionVersPion( MIL_Automate& callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    callerAutomate.GetOrderManager().CDT_GivePionMissionVersPion( pMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GiveMission
// Created: LDC 2010-12-06
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GiveMission( DEC_Decision_ABC* callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    DEC_OrdersFunctions::CDT_GivePionMission( &callerPion->GetPion().GetAutomate().GetDecision(), pMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GiveMission
// Created: GGE 2010-12-06
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GiveMissionVersPion( DEC_Decision_ABC* callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    DEC_OrdersFunctions::CDT_GivePionMissionVersPion( callerPion->GetPion().GetAutomate(), pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CreateAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CreateAutomateMission( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pAutomate, const std::string& mission   )
{
    if( !pAutomate )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pMission = callerAutomate->GetAutomate().GetOrderManager().CreateAutomateMission( pAutomate->GetAutomate(), *pMissionType );
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveAutomateMission( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    callerAutomate->GetAutomate().GetOrderManager().GiveAutomateMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveAutomateMissionToAutomat
// Created: LMT 2011-01-17
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveAutomateMissionToAutomat( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    MIL_Automate& automate = callerAutomate->GetAutomate();
    automate.GetOrderManager().GiveMissionToNewAutomat( automate, pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveMissionToAutomat
// Created: LMT 2011-12-15
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveMissionToAutomat( boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission )
        throw( "Invalid mission" );
    pMission->GetAutomate().GetOrderManager().ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::SplitFuseau
// Created: NLD 2007-04-05
// -----------------------------------------------------------------------------
std::list<MIL_Fuseau*> DEC_OrdersFunctions::SplitFuseau( const DEC_Decision_ABC* callerAutomate, unsigned int nbrSubFuseaux )
{
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    callerAutomate->GetOrderManager().GetFuseau().SplitIntoSubFuseaux( nbrSubFuseaux, subFuseaux );
    return subFuseaux;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AssignFuseauToPionMission
// Created: LMT 2010-09-22
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AssignFuseauToPionMission( MIL_Fuseau* pFuseau,  boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission || !pFuseau )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AffectFuseau( *pFuseau );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AssignFuseauToAutomateMission
// Created: NLD 2007-04-05
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AssignFuseauToAutomateMission( MIL_Fuseau* pFuseau,  boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission || !pFuseau )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AffectFuseau( *pFuseau );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AssignDirectionToAutomateMission
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AssignDirectionToAutomateMission( MT_Vector2D* pDirection,  boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission ||!pDirection )
        throw MASA_EXCEPTION( "invalid parameter." );
    pMission->AffectDirection( *pDirection );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetHigherEngagedAutomate
// Created: NLD 2007-05-09
// -----------------------------------------------------------------------------
MIL_Automate& DEC_OrdersFunctions::GetHigherEngagedAutomate( MIL_Automate& automate )
{
    if( automate.GetParentAutomate() && automate.GetParentAutomate()->IsEngaged() )
        return GetHigherEngagedAutomate( *automate.GetParentAutomate() );
    return automate;
}

namespace
{
    class FlagMissionLima : private boost::noncopyable
    {
    public:
        FlagMissionLima( unsigned int nLimaID, bool bValue ) : nLimaID_( nLimaID ), bValue_( bValue ) {}

        template< typename T >
        void operator() ( T& entity )
        {
            MIL_LimaOrder* pLima = entity.GetOrderManager().FindLima( nLimaID_ );
            if( pLima )
                pLima->Flag( bValue_ );
        }
    private:
        const unsigned int nLimaID_;
        const bool bValue_;
    };

    class FlagScheduleMissionLima : private boost::noncopyable
    {
    public:
        FlagScheduleMissionLima( unsigned int nLimaID, bool bValue ) : nLimaID_( nLimaID ), bValue_( bValue ) {}

        template< typename T >
        void operator() ( T& entity )
        {
            MIL_LimaOrder* pLima = entity.GetOrderManager().FindLima( nLimaID_ );
            if( pLima )
                pLima->FlagSchedule( bValue_ );
        }
    private:
        const unsigned int nLimaID_;
        const bool bValue_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::PionSetMissionLimaFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::PionSetMissionLimaFlag( MIL_AgentPion& caller, unsigned int limaId, bool flag )
{
    FlagMissionLima functor( limaId, flag );
    if( !caller.GetAutomate().IsEngaged() )
        functor( caller );
    else
        GetHigherEngagedAutomate( caller.GetAutomate() ).ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag( MIL_AgentPion& caller, unsigned int limaId, bool flag  )
{
    FlagScheduleMissionLima functor( limaId, flag );
    if( !caller.GetAutomate().IsEngaged() )
        functor( caller );
    else
        GetHigherEngagedAutomate( caller.GetAutomate() ).ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AutomateSetMissionLimaFlag( DEC_Decision_ABC* caller, unsigned int limaId, bool flag  )
{
    FlagMissionLima functor( limaId, flag );
    GetHigherEngagedAutomate( caller->GetAutomate() ).ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag( DEC_Decision_ABC* caller, unsigned int limaId, bool flag  )
{
    FlagScheduleMissionLima functor( limaId, flag );
    GetHigherEngagedAutomate( caller->GetAutomate() ).ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsPionMissionAvailable
// Created: MGD 2010-06-10
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::IsPionMissionAvailable( DEC_Decision_ABC* agent, std::string diaType )
{
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( diaType );
    if( agent && pMissionType )
        return agent->GetPion().GetOrderManager().IsMissionAvailable( *pMissionType );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsFragOrderAvailable
// Created: LDC 2012-09-06
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::IsFragOrderAvailable( DEC_Decision_ABC* agent, const std::string& fragorder )
{
    if( !agent )
        return false;
    const MIL_FragOrderType* fragOrderType = MIL_FragOrderType::FindByDiaType( fragorder );
    if( !fragOrderType )
        return false;
    if( fragOrderType->IsAvailableWithoutMission() )
        return agent->IsFragOrderAvailable( *fragOrderType );
    else
    {
        boost::shared_ptr< MIL_Mission_ABC > mission = agent->GetMission();
        if( !mission )
            return false;
        const MIL_MissionType_ABC& missionType = mission->GetType();
        return agent->IsFragOrderAvailableForMission( missionType, *fragOrderType );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::DEC_Mission_IsPath
// Created: LMT 2011-12-15
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::DEC_Mission_IsPath( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter )
{
    if( !pMission )
        throw MASA_EXCEPTION( "Bad mission passed to DEC_Mission_IsPath" );
    const MIL_MissionType_ABC& missionType = pMission->GetType();
    const MIL_OrderTypeParameter& orderParameterType = missionType.GetParameterType( missionType.GetParameterIndex( parameter ) );
    const MIL_ParameterType_ABC& parameterType = orderParameterType.GetType();
    if ( parameterType.GetType() == MIL_ParameterType_ABC::ePath )
        return true;
    else
        return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsAutomateMissionAvailable
// Created: LMT 2011-12-15
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::IsAutomateMissionAvailable( DEC_Decision_ABC* agent, std::string diaType )
{
    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::FindFromDiaID( diaType );
    if( agent && pMissionType )
        return agent->GetAutomate().GetOrderManager().IsMissionAvailable( *pMissionType );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetFuseau
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
const MIL_Fuseau& DEC_OrdersFunctions::GetFuseau( const DEC_Decision_ABC& caller )
{
    return caller.GetOrderManager().GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
bool DEC_OrdersFunctions::GetMissionLimaFlag( const DEC_Decision_ABC* caller, unsigned int limaID )
{
    MIL_LimaOrder* pLima = caller->GetOrderManager().FindLima( limaID );
    if( !pLima )
    {
        return false;
    }
    return pLima->IsFlagged();
}
