// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroup.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 8 $
// $Workfile: PHY_DotationGroup.h $
//
// *****************************************************************************

#ifndef __PHY_DotationGroup_h_
#define __PHY_DotationGroup_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_Dotation;
class PHY_DotationType;
class PHY_DotationGroupContainer;
class PHY_DotationCapacity;
class PHY_SupplyDotationRequestContainer;
class PHY_AmmoDotationClass;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_DotationGroup
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationGroup
{
    MT_COPYNOTALLOWED( PHY_DotationGroup )

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_Dotation* > T_DotationMap;
    typedef T_DotationMap::const_iterator                          CIT_DotationMap;
    //@}

public:
     PHY_DotationGroup( const PHY_DotationType& type, PHY_DotationGroupContainer& groupContainer );
     PHY_DotationGroup();
    ~PHY_DotationGroup();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Init
    //@{
    void ReadValues( MIL_InputArchive& archive );
    //@}

    //! @name Operations
    //@{
    void     AddCapacity   ( const PHY_DotationCapacity& capacity );
    void     RemoveCapacity( const PHY_DotationCapacity& capacity );
    MT_Float GetCapacity   ( const PHY_DotationCategory& category ) const;
    
    void     CancelConsumptionReservations ();
    MT_Float AddConsumptionReservation     ( const PHY_DotationCategory& category, MT_Float rNbr );
    void     ConsumeConsumptionReservations();    

    MT_Float AddFireReservation     ( const PHY_DotationCategory& category, MT_Float rNbr );
    void     ConsumeFireReservations();    

    MT_Float GetValue( const PHY_DotationCategory& category ) const;   
    MT_Float Consume ( const PHY_DotationCategory& category, MT_Float rNbr );
    MT_Float Supply  ( const PHY_DotationCategory& category, MT_Float rNbr );
    void     Resupply();
    //@}

    //! @name Misc
    //@{
    void NotifyTakenPrisoner();
    //@}

    //! @name Notifications
    //@{
    void NotifyDotationChanged( const PHY_Dotation& dotation );
    //@}

    //! @name Logistic - supply
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory ) const;
    void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& supplyRequest ) const;
    void ChangeDotationsValueUsingTC2( const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 ) const;
    //@}

    //! @name Accessors
    //@{
    const T_DotationMap& GetDotations() const;
    //@}

private:
    //! @name Tools
    //@{
    PHY_Dotation* GetDotation   ( const PHY_DotationCategory& dotationCategory ) const;
    PHY_Dotation& CreateDotation( const PHY_DotationCategory& dotationCategory );
    //@}

private:
    const PHY_DotationType*           pType_;
          PHY_DotationGroupContainer* pGroupContainer_;

          T_DotationMap               dotations_;
};

#include "PHY_DotationGroup.inl"

#endif // __PHY_DotationGroup_h_
