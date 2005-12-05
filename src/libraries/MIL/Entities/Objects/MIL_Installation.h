//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Installation.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_Installation.h $
//
//*****************************************************************************

#ifndef __MIL_Installation_h_
#define __MIL_Installation_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Installation : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_Installation );

public:
             MIL_Installation();
    virtual ~MIL_Installation();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_Installation.inl"

#endif // __MIL_Installation_h_
