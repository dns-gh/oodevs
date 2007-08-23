// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_Dotation.h $
// $Author: Jvt $
// $Modtime: 31/03/05 17:31 $
// $Revision: 7 $
// $Workfile: PHY_Dotation.h $
//
// *****************************************************************************

#ifndef __PHY_Dotation_h_
#define __PHY_Dotation_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_DotationGroup;
class PHY_DotationCapacity;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_Dotation
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Dotation
{
    MT_COPYNOTALLOWED( PHY_Dotation )

public:
     PHY_Dotation( const PHY_DotationCategory& category, PHY_DotationGroup& group );
     PHY_Dotation();
    ~PHY_Dotation();
  
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void     AddCapacity        ( const PHY_DotationCapacity& capacity );
    void     RemoveCapacity     ( const PHY_DotationCapacity& capacity );
    MT_Float GetCapacity        () const;

    MT_Float AddConsumptionReservation    ( MT_Float rNbr );
    void     CancelConsumptionReservation ();
    void     ConsumeConsumptionReservation();

    MT_Float AddFireReservation    ( MT_Float rNbr );
    void     ConsumeFireReservation();

    MT_Float GetValue() const;
    MT_Float Consume ( MT_Float rValue );

    void     ReadValue( xml::xistream& xis );
    void     Resupply ( MT_Float rFactor = 1. );

    bool     HasReachedSupplyThreshold() const;
    bool     NeedSupply               () const;
    MT_Float Supply                   ( MT_Float rValue );
    void     ChangeValueUsingTC2      ( MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 );

    void NotifyCaptured(); //$$$ a renommer et abstraire
    void NotifyReleased();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory() const;
    //@}

private:
    //! @name Tools
    //@{
    void SetValue( MT_Float rValue );
    //@}

private:
    const PHY_DotationCategory* pCategory_;
          PHY_DotationGroup*    pGroup_;
          MT_Float              rValue_;
          MT_Float              rCapacity_;
          MT_Float              rConsumptionReservation_;
          MT_Float              rFireReservation_;
          MT_Float              rSupplyThreshold_;
          bool                  bDotationBlocked_; // Prisoner
};

#include "PHY_Dotation.inl"

#endif // __PHY_Dotation_h_
