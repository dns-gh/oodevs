//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_DestructionPont.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:12 $
// $Revision: 6 $
// $Workfile: MIL_DestructionPont.h $
//
//*****************************************************************************

#ifndef __MIL_DestructionPont_h_
#define __MIL_DestructionPont_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_DestructionPont : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_DestructionPont );

public:
             MIL_DestructionPont( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_DestructionPont();
    virtual ~MIL_DestructionPont();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_DestructionPont.inl"

#endif // __MIL_DestructionPont_h_
