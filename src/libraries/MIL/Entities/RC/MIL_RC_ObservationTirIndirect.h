// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_ObservationTirIndirect.h $
// $Author: Nld $
// $Modtime: 9/03/05 15:20 $
// $Revision: 2 $
// $Workfile: MIL_RC_ObservationTirIndirect.h $
//
// *****************************************************************************

#ifndef __MIL_RC_ObservationTirIndirect_h_
#define __MIL_RC_ObservationTirIndirect_h_

#include "MIL.h"

#include "MIL_RC.h"

class MIL_Effect_IndirectFire;

// =============================================================================
// @class  MIL_RC_ObservationTirIndirect
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_ObservationTirIndirect : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_ObservationTirIndirect )

public:   
    MIL_RC_ObservationTirIndirect( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_ObservationTirIndirect();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;    
            void Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Effect_IndirectFire& fire ) const;
    //@}
};

#endif // __MIL_RC_ObservationTirIndirect_h_
