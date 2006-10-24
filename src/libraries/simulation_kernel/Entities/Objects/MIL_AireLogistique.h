//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_AireLogistique.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:08 $
// $Revision: 3 $
// $Workfile: MIL_AireLogistique.h $
//
//*****************************************************************************

#ifndef __MIL_AireLogistique_h_
#define __MIL_AireLogistique_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_AireLogistique : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_AireLogistique );

public:
             MIL_AireLogistique( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_AireLogistique();
    virtual ~MIL_AireLogistique();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_AireLogistique.inl"

#endif // __MIL_AireLogistique_h_
