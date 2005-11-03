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
                           static void Recon  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    template< typename T > static void IsRecon( DIA_Call_ABC& call, const T& caller );
    //@}
};

#include "DEC_KnowledgePopulationFunctions.inl"

#endif // __DEC_KnowledgePopulationFunctions_h_
