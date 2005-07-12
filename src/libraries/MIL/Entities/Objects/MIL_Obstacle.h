//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Obstacle.h $
// $Author: Nld $
// $Modtime: 29/03/05 17:49 $
// $Revision: 1 $
// $Workfile: MIL_Obstacle.h $
//
//*****************************************************************************

#ifndef __MIL_Obstacle_h_
#define __MIL_Obstacle_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Obstacle : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_Obstacle );

public:
             MIL_Obstacle( const MIL_RealObjectType& type );
    virtual ~MIL_Obstacle();

    //! @name Operations
    //@{
    virtual bool CanCollideWithAgent( const MIL_Agent_ABC& agent ) const;
    //@}
};

#include "MIL_Obstacle.inl"

#endif // __MIL_Obstacle_h_
