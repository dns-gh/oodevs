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

#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateMission.h"

//=============================================================================
// DIA MRT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_Mission_ABC* DEC_OrdersFunctions::MRT_CreatePionMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
{
    assert( pPion );

    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    assert( pMissionType );

    MIL_PionMission* pPionMission = callerAutomate.GetOrderManager().MRT_CreatePionMission( pPion->GetPion(), *pMissionType );
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
    if( !callerAutomate.GetOrderManager().GetFuseau().SplitIntoSubFuseaux( pions.size(), subFuseaux ) )
        return;

    // Affectation des fuseaux
    for( std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = ( *itPion )->GetPion();
        MIL_Fuseau::IT_FuseauPtrList itFuseau;
        for( itFuseau = subFuseaux.begin(); itFuseau != subFuseaux.end(); ++itFuseau )
        {
            MIL_Fuseau& fuseau = **itFuseau;
            if( fuseau.IsInside( pion.GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
            {
                callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, fuseau );
                break;
            }
        }

        // Pas de fuseau trouvé => prend le 1er
        if( itFuseau == subFuseaux.end() )
        {
            MIL_Fuseau& fuseau = *subFuseaux.front();
            callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, fuseau );
            subFuseaux.erase( subFuseaux.begin() );
        }
        else
            subFuseaux.erase( itFuseau );
    }
    assert( subFuseaux.empty() );
}

//=============================================================================
// Conduite (CDT)
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_CreatePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
MIL_Mission_ABC* DEC_OrdersFunctions::CDT_CreatePionMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission )
{
    assert( pPion );

    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::FindFromDiaID( mission );
    assert( pMissionType );

    MIL_PionMission* pPionMission = callerAutomate.GetOrderManager().CDT_CreatePionMission( pPion->GetPion(), *pMissionType );
    return pPionMission;
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GivePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GivePionMission( MIL_Automate& callerAutomate, MIL_Mission_ABC* pMission )
{
    callerAutomate.GetOrderManager().CDT_GivePionMission( *pMission );
}

// =============================================================================
// ADA
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CreateAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
MIL_Mission_ABC* DEC_OrdersFunctions::CreateAutomateMission( MIL_Automate& callerAutomate, DEC_Decision_ABC* pAutomate, const std::string& mission   )
{
    assert( pAutomate );

    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::FindFromDiaID( mission );
    assert( pMissionType );

    MIL_AutomateMission* pMission = callerAutomate.GetOrderManager().CreateAutomateMission( pAutomate->GetAutomate(), *pMissionType );
    return pMission;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GiveAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::GiveAutomateMission( MIL_AutomateMission* pMission, MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().GiveAutomateMission( *pMission );
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
// Name: DEC_OrdersFunctions::AssignFuseauToAutomateMission
// Created: NLD 2007-04-05
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AssignFuseauToAutomateMission( MIL_Fuseau* pFuseau, MIL_AutomateMission* pMission )
{
    assert( pMission && pFuseau );

    pMission->AffectFuseau( *pFuseau );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AssignDirectionToAutomateMission
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AssignDirectionToAutomateMission( MT_Vector2D* pDirection, MIL_AutomateMission* pMission )
{
    assert( pMission && pDirection );

    pMission->AffectDirection( *pDirection );
}

// =============================================================================
// LIMAS
// =============================================================================

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
    class FlagMissionLima
    {   
    public:
        FlagMissionLima( uint nLimaID, bool bValue ) : nLimaID_( nLimaID ), bValue_( bValue ) {}

        template< typename T >
        void operator() ( T& entity )
        {
            MIL_LimaOrder* pLima = entity.GetOrderManager().FindLima( nLimaID_ );
            if( pLima )
                pLima->Flag( bValue_ );
        }
    private:
        const uint nLimaID_;
        const bool bValue_;
    };

    class FlagScheduleMissionLima
    {   
    public:
        FlagScheduleMissionLima( uint nLimaID, bool bValue ) : nLimaID_( nLimaID ), bValue_( bValue ) {}

        template< typename T >
        void operator() ( T& entity )
        {
            MIL_LimaOrder* pLima = entity.GetOrderManager().FindLima( nLimaID_ );
            if( pLima )
                pLima->FlagSchedule( bValue_ );
        }
    private:
        const uint nLimaID_;
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

