//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Barricade.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 6 $
// $Workfile: MIL_Barricade.h $
//
//*****************************************************************************

#ifndef __MIL_Barricade_h_
#define __MIL_Barricade_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Barricade : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_Barricade );

public:
             MIL_Barricade( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_Barricade();
    virtual ~MIL_Barricade();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_Barricade.inl"

#endif // __MIL_Barricade_h_
