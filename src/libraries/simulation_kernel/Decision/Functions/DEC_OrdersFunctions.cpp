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

#include "MIL_AgentServer.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Decision/DEC_Tools.h"

//=============================================================================
// DIA MRT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_CreatePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    const MIL_PionMissionType* pMissionType = MIL_PionMissionType::FindFromDiaID( call.GetParameter( 1 ).ToId() );
    assert( pMissionType );

    MIL_PionMission* pPionMission = callerAutomate.GetOrderManager().MRT_CreatePionMission( pPion->GetPion(), *pMissionType );
    call.GetResult().SetValue( *pPionMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_Validate( DIA_Call_ABC& /*call*/, MIL_Automate& callerAutomate )
{
    callerAutomate.GetOrderManager().MRT_Validate();
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::MRT_AffectFuseaux
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::MRT_AffectFuseaux( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();

    //$$$ NAZE
    assert( callerAutomate.IsEngaged() );

    // Découpage
    MIL_Fuseau::T_FuseauPtrList subFuseaux;
    if( !callerAutomate.GetFuseau().SplitIntoSubFuseaux( pions.size(), subFuseaux ) )
        return;

    // Affectation des fuseaux
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        for( MIL_Fuseau::IT_FuseauPtrList itFuseau = subFuseaux.begin(); itFuseau != subFuseaux.end(); ++itFuseau )
        {
            MIL_Fuseau& fuseau = **itFuseau;
            if( fuseau.IsInside( pion.GetRole< PHY_RolePion_Location >().GetPosition() ) )
            {
                callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, fuseau );
                subFuseaux.erase( itFuseau );
                break;
            }
        }

        // Pas de fuseau trouvé => prend le 1er
        if( itFuseau == subFuseaux.end() )
        {
            MIL_Fuseau& fuseau = **subFuseaux.begin();
            callerAutomate.GetOrderManager().MRT_SetFuseauForPion( pion, fuseau );
            subFuseaux.erase( subFuseaux.begin() );
        }
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
void DEC_OrdersFunctions::CDT_CreatePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    // Pion
    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    assert( pPion );

    // Instanciate and check the new mission
    const MIL_PionMissionType* pMissionType = MIL_PionMissionType::FindFromDiaID( call.GetParameter( 1 ).ToId() );
    assert( pMissionType );

    MIL_PionMission* pPionMission = callerAutomate.GetOrderManager().CDT_CreatePionMission( pPion->GetPion(), *pMissionType );
    call.GetResult().SetValue( *pPionMission );
}

//-----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::CDT_GivePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void DEC_OrdersFunctions::CDT_GivePionMission( DIA_Call_ABC& call, MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypeMissionPion( call.GetParameter( 0 ) ) );

    MIL_PionMission* pPionMission = call.GetParameter( 0 ).ToUserObject( pPionMission );
    assert( pPionMission );

    callerAutomate.GetOrderManager().CDT_GivePionMission( *pPionMission );
}
// =============================================================================
// LIMAS
// =============================================================================

namespace 
{
    class FlagMissionLima
    {   
    public:
        FlagMissionLima( uint nLimaID, bool bValue ) : nLimaID_( nLimaID ), bValue_( bValue ) {}

        template< typename T >
        void operator() ( T& entity )
        {
            MIL_LimaOrder* pLima = entity.FindLima( nLimaID_ );
            if( pLima )
                pLima->Flag( bValue_ );
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
void DEC_OrdersFunctions::PionSetMissionLimaFlag( DIA_Call_ABC& call, MIL_AgentPion& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );

    FlagMissionLima functor( (uint)call.GetParameter( 0 ).ToPtr(), call.GetParameter( 1 ).ToBool() );
    if( !caller.GetAutomate().IsEngaged() )
        functor( caller );
    else
        caller.GetAutomate().ApplyOnHierarchy( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::AutomateSetMissionLimaFlag
// Created: NLD 2007-05-08
// -----------------------------------------------------------------------------
void DEC_OrdersFunctions::AutomateSetMissionLimaFlag( DIA_Call_ABC& call, MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );

    FlagMissionLima functor( (uint)call.GetParameter( 0 ).ToPtr(), call.GetParameter( 1 ).ToBool() );
    caller.ApplyOnHierarchy( functor );
}
