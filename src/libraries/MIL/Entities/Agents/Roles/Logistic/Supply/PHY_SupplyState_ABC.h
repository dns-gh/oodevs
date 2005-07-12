// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyState_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 12:11 $
// $Revision: 2 $
// $Workfile: PHY_SupplyState_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyState_ABC_h_
#define __PHY_SupplyState_ABC_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"

// =============================================================================
// @class  PHY_SupplyState_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyState_ABC
{
    MT_COPYNOTALLOWED( PHY_SupplyState_ABC )

public:
             PHY_SupplyState_ABC();
    virtual ~PHY_SupplyState_ABC() = 0;

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

protected:
    const uint nID_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "PHY_SupplyState_ABC.inl"

#endif // __PHY_SupplyState_ABC_h_
