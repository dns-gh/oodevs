// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgePopulationFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 11:38 $
// $Revision: 11 $
// $Workfile: DEC_KnowledgePopulationFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgePopulationFunctions_h_
#define __DEC_KnowledgePopulationFunctions_h_

#include "MIL.h"

class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgePopulationFunctions
{
public:
    //! @name Functions
    //@{
                           static void GetDangerosity    ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void Recon             ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
    template< typename T > static void IsRecon           ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsInZone          ( DIA_Call_ABC& call, const T& caller );
                           static void IsPerceivedByAgent( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void ClosestPoint      ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void Secure            ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void SecuringPoint     ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void IsEnemy           ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
                           static void Exterminate       ( DIA_Call_ABC& call, const MIL_AgentPion& caller );
    //@}
};

#include "DEC_KnowledgePopulationFunctions.inl"

#endif // __DEC_KnowledgePopulationFunctions_h_
