// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_Breakdown.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:17 $
// $Revision: 4 $
// $Workfile: PHY_Breakdown.h $
//
// *****************************************************************************

#ifndef __PHY_Breakdown_h_
#define __PHY_Breakdown_h_

#include "MIL.h"
#include "PHY_BreakdownType.h"

// =============================================================================
// @class  PHY_Breakdown
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Breakdown : private boost::noncopyable
{

public:
    //! @name Contruction
    //@{
     explicit PHY_Breakdown( const PHY_BreakdownType& type );
              PHY_Breakdown();
             ~PHY_Breakdown();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name accessors
    //@{
          uint                          GetTheoricRepairTime() const;
          uint                          GetRepairTime       () const;
          uint                          GetID               () const;
          bool                          AffectMobility      () const;
          bool                          AffectElectronic    () const;
    const PHY_MaintenanceLevel&         GetMaintenanceLevel () const;
    const PHY_BreakdownType::T_PartMap& GetNeededParts      () const;
    //@}


private:
    const PHY_BreakdownType* pType_;
    const uint               nRepairTime_;
};

#endif // __PHY_Breakdown_h_
