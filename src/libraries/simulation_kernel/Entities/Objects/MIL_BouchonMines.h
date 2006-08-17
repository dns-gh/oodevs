//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_BouchonMines.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:10 $
// $Revision: 6 $
// $Workfile: MIL_BouchonMines.h $
//
//*****************************************************************************

#ifndef __MIL_BouchonMines_h_
#define __MIL_BouchonMines_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_BouchonMines : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_BouchonMines );

public:
             MIL_BouchonMines();
    virtual ~MIL_BouchonMines();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    //@}
    
private:
    //! @name Events
    //@{
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_BouchonMines.inl"

#endif // __MIL_BouchonMines_h_
