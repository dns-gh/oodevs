//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FosseAntichar.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:16 $
// $Revision: 6 $
// $Workfile: MIL_FosseAntichar.h $
//
//*****************************************************************************

#ifndef __MIL_FosseAntiChar_h_
#define __MIL_FosseAntiChar_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_FosseAntiChar : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_FosseAntiChar );

public:
    MIL_FosseAntiChar();
    virtual ~MIL_FosseAntiChar();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    // @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    //@}
};

#include "MIL_FosseAntiChar.inl"

#endif // __MIL_FosseAntiChar_h_
