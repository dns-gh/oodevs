//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBTypeAutomate.h $
// $Author: Fbd $
// $Modtime: 28/07/03 18:48 $
// $Revision: 1 $
// $Workfile: MOS_ODBTypeAutomate.h $
//
//*****************************************************************************

#ifndef __MOS_ODBTypeAutomate_h_
#define __MOS_ODBTypeAutomate_h_

#include "MOS_Types.h"

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ODBTypeAutomate
{
    MT_COPYNOTALLOWED( MOS_ODBTypeAutomate );

public:
    MOS_ODBTypeAutomate( const std::string& sName, const std::string& sSymbol );
    virtual ~MOS_ODBTypeAutomate();


    const std::string& GetName() const;
    const std::string& GetSymbol() const;

private:
    std::string sName_;
    std::string sSymbol_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ODBTypeAutomate.inl"
#endif

#endif // __MOS_ODBTypeAutomate_h_
