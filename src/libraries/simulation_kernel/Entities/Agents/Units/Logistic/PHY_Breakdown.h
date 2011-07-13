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
    //! @name Contructors/Destructor
    //@{
    explicit PHY_Breakdown( const PHY_BreakdownType& type );
             PHY_Breakdown();
    virtual ~PHY_Breakdown();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name accessors
    //@{
          unsigned int                  GetTheoricRepairTime() const;
          unsigned int                  GetRepairTime       () const;
          unsigned int                  GetID               () const;
          bool                          AffectMobility      () const;
          bool                          AffectElectronic    () const;
    const PHY_MaintenanceLevel&         GetMaintenanceLevel () const;
    const PHY_BreakdownType::T_PartMap& GetNeededParts      () const;
    const PHY_BreakdownType&            GetType             () const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_BreakdownType* pType_;
    const unsigned int nRepairTime_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_Breakdown )

#endif // __PHY_Breakdown_h_
