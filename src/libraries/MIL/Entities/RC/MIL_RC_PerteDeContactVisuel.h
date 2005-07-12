// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_PerteDeContactVisuel.h $
// $Author: Nld $
// $Modtime: 9/03/05 15:20 $
// $Revision: 3 $
// $Workfile: MIL_RC_PerteDeContactVisuel.h $
//
// *****************************************************************************

#ifndef __MIL_RC_PerteDeContactVisuel_h_
#define __MIL_RC_PerteDeContactVisuel_h_

#include "MIL.h"

#include "MIL_RC.h"

// =============================================================================
// @class  MIL_RC_PerteDeContactVisuel
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_PerteDeContactVisuel : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_PerteDeContactVisuel )

public:   
    MIL_RC_PerteDeContactVisuel( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_PerteDeContactVisuel();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
    //@}
};

#endif // __MIL_RC_PerteDeContactVisuel_h_
