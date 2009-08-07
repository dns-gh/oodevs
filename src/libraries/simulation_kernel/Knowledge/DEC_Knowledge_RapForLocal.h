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

#include "DEC_Knowledge_RapFor_ABC.h"
#include "DEC_Knowledge_Def.h"

class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-04-07
// =============================================================================
class DEC_Knowledge_RapForLocal : public DEC_Knowledge_RapFor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Knowledge_RapForLocal( const MIL_AgentPion& pion );
             DEC_Knowledge_RapForLocal();
    virtual ~DEC_Knowledge_RapForLocal();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

    //! @name Accessors
    //@{
    const T_ConstKnowledgeAgentVector& GetDangerousEnemies();
    //@}

private:
    const MIL_AgentPion*              pPion_;
          T_ConstKnowledgeAgentVector dangerousEnemies_;
};

#endif // __DEC_Knowledge_RapForLocal_h_
