//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationCanon.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:12 $
// $Revision: 6 $
// $Workfile: MIL_ZoneImplantationCanon.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneImplantationCanon_h_
#define __MIL_ZoneImplantationCanon_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneImplantationCanon : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneImplantationCanon );

public:
             MIL_ZoneImplantationCanon();
    virtual ~MIL_ZoneImplantationCanon();

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

#include "MIL_ZoneImplantationCanon.inl"

#endif // __MIL_ZoneImplantationCanon_h_
