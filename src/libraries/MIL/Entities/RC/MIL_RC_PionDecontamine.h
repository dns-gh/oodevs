// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_PionDecontamine.h $
// $Author: Nld $
// $Modtime: 26/04/05 11:52 $
// $Revision: 2 $
// $Workfile: MIL_RC_PionDecontamine.h $
//
// *****************************************************************************

#ifndef __MIL_RC_PionDecontamine_h_
#define __MIL_RC_PionDecontamine_h_

#include "MIL.h"

#include "MIL_RC.h"

// =============================================================================
// @class  MIL_RC_PionDecontamine
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_PionDecontamine : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_PionDecontamine )

public:   
    MIL_RC_PionDecontamine( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_PionDecontamine();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
            void Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Agent_ABC& param ) const;
    //@}
};

#endif // __MIL_RC_PionDecontamine_h_
