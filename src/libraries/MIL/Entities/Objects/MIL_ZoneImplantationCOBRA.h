//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationCOBRA.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:12 $
// $Revision: 6 $
// $Workfile: MIL_ZoneImplantationCOBRA.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneImplantationCOBRA_h_
#define __MIL_ZoneImplantationCOBRA_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneImplantationCOBRA : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneImplantationCOBRA );

public:
             MIL_ZoneImplantationCOBRA();
    virtual ~MIL_ZoneImplantationCOBRA();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}

protected:
    virtual void ProcessAgentInside ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Agent_ABC& agent );
};

#include "MIL_ZoneImplantationCOBRA.inl"

#endif // __MIL_ZoneImplantationCOBRA_h_
