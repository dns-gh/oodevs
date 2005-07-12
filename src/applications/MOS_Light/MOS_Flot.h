//*****************************************************************************
//
// $Created: NLD 2003-06-04 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Flot.h $
// $Author: Jdy $
// $Modtime: 6/06/03 11:13 $
// $Revision: 2 $
// $Workfile: MOS_Flot.h $
//
//*****************************************************************************

#ifndef __MOS_Flot_h_
#define __MOS_Flot_h_

#include "MOS_Types.h"

class GFX_Color;

//=============================================================================
// Created: NLD 2003-06-04
//=============================================================================
class MOS_Flot
{
public:
     MOS_Flot();
    ~MOS_Flot();

    //-------------------------------------------------------------------------
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void Draw(GFX_Color areaColor);
    //@}

    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    void OnReceiveMsgUpdate( DIN::DIN_Input& input );
    //@}

private:
    
    void Reset();

private:
    T_PointVector       limits_;
    T_PointVector       points_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_Flot.inl"
#endif


#endif // __MOS_Flot_h_
