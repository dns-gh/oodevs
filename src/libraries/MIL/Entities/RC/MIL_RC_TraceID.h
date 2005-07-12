// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_TraceID.h $
// $Author: Nld $
// $Modtime: 9/03/05 18:33 $
// $Revision: 1 $
// $Workfile: MIL_RC_TraceID.h $
//
// *****************************************************************************

#ifndef __MIL_RC_TraceID_h_
#define __MIL_RC_TraceID_h_

#include "MIL.h"

#include "MIL_RC.h"

// =============================================================================
// @class  MIL_RC_TraceID
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_TraceID : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_TraceID )

public:   
    MIL_RC_TraceID( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_TraceID();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
    //@}
};

#endif // __MIL_RC_TraceID_h_
