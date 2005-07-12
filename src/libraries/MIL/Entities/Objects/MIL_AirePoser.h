//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_AirePoser.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:09 $
// $Revision: 4 $
// $Workfile: MIL_AirePoser.h $
//
//*****************************************************************************

#ifndef __MIL_AirePoser_h_
#define __MIL_AirePoser_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_AirePoser : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_AirePoser );

public:
             MIL_AirePoser();
    virtual ~MIL_AirePoser();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_AirePoser.inl"

#endif // __MIL_AirePoser_h_
