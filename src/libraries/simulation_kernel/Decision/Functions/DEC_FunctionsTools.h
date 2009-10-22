// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_FunctionsTools.h $
// $Author: Nld $
// $Modtime: 22/10/04 16:32 $
// $Revision: 2 $
// $Workfile: DEC_FunctionsTools.h $
//
// *****************************************************************************

#ifndef __DEC_FunctionsTools_h_
#define __DEC_FunctionsTools_h_

#include "MIL.h"

class MIL_KnowledgeGroup;
class MIL_Army_ABC;
class MIL_AgentPion;
class MIL_Object_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FunctionsTools
{
public:
    //! @name Functions
    //@{
    static MIL_Object_ABC*           GetPopulationKnowledgeObjectFromDia( unsigned int nID );
    //@}
};

#endif // __DEC_FunctionsTools_h_
