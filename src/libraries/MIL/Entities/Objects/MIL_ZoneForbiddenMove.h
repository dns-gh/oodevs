//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneForbiddenMove.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:26 $
// $Revision: 3 $
// $Workfile: MIL_ZoneForbiddenMove.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneForbiddenMove_h_
#define __MIL_ZoneForbiddenMove_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneForbiddenMove : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneForbiddenMove );

public:
             MIL_ZoneForbiddenMove();
    virtual ~MIL_ZoneForbiddenMove();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}

    //! @name Operations
    //@{
    virtual bool CanCollideWithAgent( const MIL_Agent_ABC& agent ) const;
    //@}
};

#include "MIL_ZoneForbiddenMove.inl"

#endif // __MIL_ZoneForbiddenMove_h_
