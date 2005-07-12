//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PlotRavitaillement.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 5 $
// $Workfile: MIL_PlotRavitaillement.h $
//
//*****************************************************************************

#ifndef __MIL_PlotRavitaillement_h_
#define __MIL_PlotRavitaillement_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_PlotRavitaillement : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_PlotRavitaillement );

public:
             MIL_PlotRavitaillement();
    virtual ~MIL_PlotRavitaillement();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_PlotRavitaillement.inl"

#endif // __MIL_PlotRavitaillement_h_
