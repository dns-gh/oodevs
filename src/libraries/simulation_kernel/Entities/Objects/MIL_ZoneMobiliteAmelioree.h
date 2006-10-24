//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMobiliteAmelioree.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 4 $
// $Workfile: MIL_ZoneMobiliteAmelioree.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneMobiliteAmelioree_h_
#define __MIL_ZoneMobiliteAmelioree_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneMobiliteAmelioree : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneMobiliteAmelioree );

public:
             MIL_ZoneMobiliteAmelioree( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneMobiliteAmelioree();
    virtual ~MIL_ZoneMobiliteAmelioree();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_ZoneMobiliteAmelioree.inl"

#endif // __MIL_ZoneMobiliteAmelioree_h_
