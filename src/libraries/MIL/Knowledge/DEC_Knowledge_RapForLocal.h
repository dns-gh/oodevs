// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapForLocal.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_RapForLocal.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_RapForLocal_h_
#define __DEC_Knowledge_RapForLocal_h_

#include "MIL.h"

#include "DEC_Knowledge_RapFor_ABC.h"

class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-04-07
// =============================================================================
class DEC_Knowledge_RapForLocal : public DEC_Knowledge_RapFor_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_RapForLocal );

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_RapForLocal();
    virtual ~DEC_Knowledge_RapForLocal();
    //@}

    //! @name Operations
    //@{
    void Update( const MIL_AgentPion& pion, const T_KnowledgeAgentVector& enemies, const T_KnowledgeAgentVector& friends );
    //@}
};

#include "DEC_Knowledge_RapForLocal.inl"

#endif // __DEC_Knowledge_RapForLocal_h_
