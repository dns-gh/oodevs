//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Eboulement.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:13 $
// $Revision: 6 $
// $Workfile: MIL_Eboulement.h $
//
//*****************************************************************************

#ifndef __MIL_Eboulement_h_
#define __MIL_Eboulement_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Eboulement : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_Eboulement );

public:
             MIL_Eboulement();
    virtual ~MIL_Eboulement();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_Eboulement.inl"

#endif // __MIL_Eboulement_h_
