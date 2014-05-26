// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
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
class PHY_Dotation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_Dotation( const PHY_DotationCategory& category, PHY_DotationGroup& group, bool bInfiniteDotations );
             PHY_Dotation();
    virtual ~PHY_Dotation();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void   AddCapacity        ( const PHY_DotationCapacity& capacity, double quantity );
    double RemoveCapacity     ( const PHY_DotationCapacity& capacity );
    double GetCapacity        () const;

    double AddConsumptionReservation    ( double rNbr );
    void   CancelConsumptionReservation ();
    void   ConsumeConsumptionReservation();
    void   UpdateSupplyNeeded();

    double AddFireReservation    ( double rNbr );
    void   ConsumeFireReservation();

    double GetValue() const;
    double Consume ( double rValue );

    void   ReadValue( xml::xistream& xis );
    void   Resupply ( double rFactor = 1., bool withLog = false );
    void   ChangeDotation( unsigned int number, float lowThreshold, float highThreshold );

    bool   HasReachedLowThreshold() const;
    bool   NeedSupply               () const;
    double Supply                   ( double rValue );
    void   SetValueUsingTC2         ( double newValue, MIL_AutomateLOG& tc2 );

    void NotifyCaptured(); //$$$ a renommer et abstraire
    void NotifyReleased();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory() const;
    double GetLowThresholdPercentage() const;
    double GetHighThresholdPercentage() const;
    //@}

private:
    //! @name Tools
    //@{
    void SetValue( double rValue );
    bool IsJammed() const;
    void NotifySupplyNeeded();
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory* pCategory_;
    PHY_DotationGroup* pGroup_;
    double rValue_;
    double rRequestedValue_;
    double rLastValueSent_;
    double rCapacity_;
    double rConsumptionReservation_;
    double rFireReservation_;
    double rLowThreshold_;
    double rHighThreshold_;
    bool bNotified_;
    bool bDotationBlocked_; // Prisoner
    bool bInfiniteDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_Dotation )

#endif // __PHY_Dotation_h_
