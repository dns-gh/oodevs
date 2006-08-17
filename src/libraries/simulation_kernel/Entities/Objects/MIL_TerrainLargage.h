//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_TerrainLargage.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:23 $
// $Revision: 3 $
// $Workfile: MIL_TerrainLargage.h $
//
//*****************************************************************************

#ifndef __MIL_TerrainLargage_h_
#define __MIL_TerrainLargage_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_TerrainLargage : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_TerrainLargage );

public:
             MIL_TerrainLargage();
    virtual ~MIL_TerrainLargage();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_TerrainLargage.inl"

#endif // __MIL_TerrainLargage_h_
