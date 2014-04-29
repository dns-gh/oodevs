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
#include "Decision/DEC_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::MRT_CreatePionMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
{
    if( !pPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = callerAutomate.GetOrderManager().MRT_CreatePionMission( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_Validate( MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().MRT_Validate();
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_AffectFuseaux
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_AffectFuseaux( MIL_Automate& callerAutomate, std::vector< DEC_Decision_ABC* > pions )
{
    //$$$ NAZE
    assert( callerAutomate.IsEngaged() );
    // Découpage
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    if( !callerAutomate.GetOrderManager().GetFuseau().SplitIntoSubFuseaux( static_cast< unsigned int >( pions.size() ), subFuseaux ) )
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
                callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, *fuseau );
                break;
            }
        }
        // Pas de fuseau trouvé => prend le 1er
        if( itFuseau == subFuseaux.end() )
        {
            MIL_Fuseau* fuseau = subFuseaux.front();
            if( !fuseau )
                throw MASA_EXCEPTION( "invalid parameter." );
            callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, *fuseau );
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
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CDT_CreatePionMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
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
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = callerAutomate.GetOrderManager().CDT_CreatePionMission( pPion->GetPion(), *pMissionType );
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
void DEC_OrdersFunctions::CDT_GivePionMission( MIL_Automate& callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    callerAutomate.GetOrderManager().CDT_GivePionMission( pMission );
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
void DEC_OrdersFunctions::CDT_GiveMission( MIL_AgentPion& callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    DEC_OrdersFunctions::CDT_GivePionMission( callerPion.GetAutomate(), pMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GiveMission
// Created: GGE 2010-12-06
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GiveMissionVersPion( MIL_AgentPion& callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    DEC_OrdersFunctions::CDT_GivePionMissionVersPion( callerPion.GetAutomate(), pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CreateAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_OrdersFunctions::CreateAutomateMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pAutomate, const std::string& mission   )
{
    if( !pAutomate )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::FindFromDiaID( mission );
    if( !pMissionType )
        throw MASA_EXCEPTION( "invalid parameter." );
    boost::shared_ptr< MIL_Mission_ABC > pMission = callerAutomate.GetOrderManager().CreateAutomateMission( pAutomate->GetAutomate(), *pMissionType );
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveAutomateMission( boost::shared_ptr< MIL_Mission_ABC > pMission, MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().GiveAutomateMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveAutomateMissionToAutomat
// Created: LMT 2011-01-17
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveAutomateMissionToAutomat( boost::shared_ptr< MIL_Mission_ABC > pMission, MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().GiveMissionToNewAutomat( callerAutomate, pMission );
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
std::list<MIL_Fuseau*> DEC_OrdersFunctions::SplitFuseau( MIL_Automate& callerAutomate, unsigned int nbrSubFuseaux )
{
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    callerAutomate.GetOrderManager().GetFuseau().SplitIntoSubFuseaux( nbrSubFuseaux, subFuseaux );
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
void DEC_OrdersFunctions::AutomateSetMissionLimaFlag( MIL_Automate& caller, unsigned int limaId, bool flag  )
{
    FlagMissionLima functor( limaId, flag );
    GetHigherEngagedAutomate( caller ).ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag( MIL_Automate& caller, unsigned int limaId, bool flag  )
{
    FlagScheduleMissionLima functor( limaId, flag );
    GetHigherEngagedAutomate( caller ).ApplyOnHierarchy( functor );
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
