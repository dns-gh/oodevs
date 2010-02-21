// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyState_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 12:11 $
// $Revision: 2 $
// $Workfile: PHY_SupplyState_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyState_ABC_h_
#define __PHY_SupplyState_ABC_h_

#include "MIL.h"
#include "Tools/MIL_IDManager.h"

// =============================================================================
// @class  PHY_SupplyState_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyState_ABC : private boost::noncopyable
{

public:
             PHY_SupplyState_ABC();
    virtual ~PHY_SupplyState_ABC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

protected:
    const unsigned int nID_;
    const unsigned int nCreationTick_;

private:
    static MIL_IDManager idManager_;
};

#endif // __PHY_SupplyState_ABC_h_
