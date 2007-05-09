//*****************************************************************************
//
// $Created: JDY 03-03-17 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Shared.h $
// $Author: Jdy $
// $Modtime: 17/03/03 18:02 $
// $Revision: 1 $
// $Workfile: MT_Shared.h $
//
//*****************************************************************************

#ifndef __MT_Shared_h_
#define __MT_Shared_h_


//*****************************************************************************
// Created: JDY 03-03-17
//*****************************************************************************
struct MT_Shared
{
    MT_Shared()		{ count_ = 1; }
    void ref()		{ count_++; }
    bool deref()	{ return !--count_; }
    uint count_;
};

#endif // __MT_Shared_h_