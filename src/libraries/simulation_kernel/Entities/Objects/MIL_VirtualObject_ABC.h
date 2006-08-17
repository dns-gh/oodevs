//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObject_ABC.h $
// $Author: Nld $
// $Modtime: 26/10/04 18:18 $
// $Revision: 1 $
// $Workfile: MIL_VirtualObject_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_VirtualObject_ABC_h_
#define __MIL_VirtualObject_ABC_h_

#include "MIL.h"

#include "MIL_Object_ABC.h"

class MIL_VirtualObjectType;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_VirtualObject_ABC : public MIL_Object_ABC
{
    MT_COPYNOTALLOWED( MIL_VirtualObject_ABC );

public:
             MIL_VirtualObject_ABC( const MIL_VirtualObjectType& type );
    virtual ~MIL_VirtualObject_ABC();

    //! @name Accessors
    //@{
    virtual bool IsReal() const;
    //@}

protected:
    const MIL_VirtualObjectType& type_; 
};

#include "MIL_VirtualObject_ABC.inl"

#endif // __MIL_VirtualObject_ABC_h_
