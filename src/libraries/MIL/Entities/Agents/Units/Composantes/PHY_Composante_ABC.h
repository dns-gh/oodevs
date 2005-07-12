// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_Composante_ABC.h $
// $Author: Jvt $
// $Modtime: 1/04/05 10:17 $
// $Revision: 4 $
// $Workfile: PHY_Composante_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Composante_ABC_h_
#define __PHY_Composante_ABC_h_

#include "MIL.h"

class PHY_ComposanteType_ABC;


// =============================================================================
// @class  PHY_Composante_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Composante_ABC
{
    MT_COPYNOTALLOWED( PHY_Composante_ABC )

public:
             PHY_Composante_ABC();
    virtual ~PHY_Composante_ABC();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    virtual const PHY_ComposanteType_ABC& GetType            () const = 0;
    virtual bool                          CanFire            () const = 0;
    virtual bool                          CanFireWhenUnloaded() const = 0;
    virtual uint                          GetMajorScore      () const = 0;
    //@}
};

#include "PHY_Composante_ABC.inl"

#endif // __PHY_Composante_ABC_h_
