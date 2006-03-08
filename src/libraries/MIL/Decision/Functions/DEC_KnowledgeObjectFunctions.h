// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeObjectFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 11:38 $
// $Revision: 11 $
// $Workfile: DEC_KnowledgeObjectFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeObjectFunctions_h_
#define __DEC_KnowledgeObjectFunctions_h_

#include "MIL.h"

class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeObjectFunctions
{
public:
    //! @name Functions
    //@{
    template< typename T > static void GetSiteFranchissementWidth		( DIA_Call_ABC& call, const T& caller );
	template< typename T > static void IsSiteFranchissementBanksToFitOut( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetType							( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetLocalisation					( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsBypassed						( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsPrepared   					( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsKnowledgeValid					( DIA_Call_ABC& call, const T& caller );

                           static void QueueForDecontamination			( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
                           static void CanBeAnimated                    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
                           static void CanBeOccupied                    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
                           static void DecontaminateZone                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
                           static void DamageObject                     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
                           
                           static void SetExitingPopulationDensity      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
                           static void ResetExitingPopulationDensity    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

                           static void Recon							( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    template< typename T > static void IsRecon							( DIA_Call_ABC& call, const T& caller );

                           static void EquipLogisticRoute				( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    template< typename T > static void IsLogisticRouteEquipped			( DIA_Call_ABC& call, const T& caller );
    //@}
};

#include "DEC_KnowledgeObjectFunctions.inl"

#endif // __DEC_KnowledgeObjectFunctions_h_
