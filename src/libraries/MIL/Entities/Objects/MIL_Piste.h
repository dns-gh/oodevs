//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Piste.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:18 $
// $Revision: 4 $
// $Workfile: MIL_Piste.h $
//
//*****************************************************************************

#ifndef __MIL_Piste_h_
#define __MIL_Piste_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Piste : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_Piste );

public:
             MIL_Piste();
    virtual ~MIL_Piste();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_Piste.inl"

#endif // __MIL_Piste_h_
