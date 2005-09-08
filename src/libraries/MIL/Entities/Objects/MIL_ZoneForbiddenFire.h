//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneForbiddenFire.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:25 $
// $Revision: 3 $
// $Workfile: MIL_ZoneForbiddenFire.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneForbiddenFire_h_
#define __MIL_ZoneForbiddenFire_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneForbiddenFire : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneForbiddenFire );

public:
             MIL_ZoneForbiddenFire();
    virtual ~MIL_ZoneForbiddenFire();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}

private:
    //! @name Operations
    //@{
    virtual bool CanInteractWithAgent( const MIL_Agent_ABC& agent ) const;
    //@}
};

#include "MIL_ZoneForbiddenFire.inl"

#endif // __MIL_ZoneForbiddenFire_h_
