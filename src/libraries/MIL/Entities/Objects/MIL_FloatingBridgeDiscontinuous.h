//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridgeDiscontinuous.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_FloatingBridgeDiscontinuous.h $
//
//*****************************************************************************

#ifndef __MIL_FloatingBridgeDiscontinuous_h_
#define __MIL_FloatingBridgeDiscontinuous_h_

#include "MIL.h"

#include "MIL_FloatingBridge_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_FloatingBridgeDiscontinuous : public MIL_FloatingBridge_ABC
{
    MT_COPYNOTALLOWED( MIL_FloatingBridgeDiscontinuous );

public:
             MIL_FloatingBridgeDiscontinuous();
    virtual ~MIL_FloatingBridgeDiscontinuous();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}
};

#include "MIL_FloatingBridgeDiscontinuous.inl"

#endif // __MIL_FloatingBridgeDiscontinuous_h_
