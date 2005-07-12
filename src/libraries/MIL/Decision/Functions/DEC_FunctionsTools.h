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

class DIA_Variable_ABC;
class MIL_KnowledgeGroup;
class MIL_Army;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FunctionsTools
{
public:
    //! @name Functions
    //@{
    static DEC_Knowledge_Agent*  GetKnowledgeAgentFromDia ( const DIA_Variable_ABC& diaVar, const MIL_KnowledgeGroup& caller );
    static DEC_Knowledge_Object* GetKnowledgeObjectFromDia( const DIA_Variable_ABC& diaVar, const MIL_Army& caller );
    //@}
};

#endif // __DEC_FunctionsTools_h_
