//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBTypePion.h $
// $Author: Fbd $
// $Modtime: 28/07/03 18:37 $
// $Revision: 1 $
// $Workfile: MOS_ODBTypePion.h $
//
//*****************************************************************************

#ifndef __MOS_ODBTypePion_h_
#define __MOS_ODBTypePion_h_

#include "MOS_Types.h"

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBTypePion
{
    MT_COPYNOTALLOWED( MOS_ODBTypePion );

public:
    MOS_ODBTypePion( const std::string& sName, const std::string& sSymbol );
    virtual ~MOS_ODBTypePion();


    const std::string& GetName() const;
    const std::string& GetSymbol() const;

private:
    std::string sName_;
    std::string sSymbol_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBTypePion.inl"
#endif

#endif // __MOS_ODBTypePion_h_
