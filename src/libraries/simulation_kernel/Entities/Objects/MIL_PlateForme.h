//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PlateForme.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 4 $
// $Workfile: MIL_PlateForme.h $
//
//*****************************************************************************

#ifndef __MIL_PlateForme_h_
#define __MIL_PlateForme_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_PlateForme : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_PlateForme );

public:
             MIL_PlateForme( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_PlateForme();
    virtual ~MIL_PlateForme();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_PlateForme.inl"

#endif // __MIL_PlateForme_h_
