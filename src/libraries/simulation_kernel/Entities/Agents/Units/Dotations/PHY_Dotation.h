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
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void   AddCapacity        ( const PHY_DotationCapacity& capacity );
    void   RemoveCapacity     ( const PHY_DotationCapacity& capacity );
    double GetCapacity        () const;

    double AddConsumptionReservation    ( double rNbr );
    void   CancelConsumptionReservation ();
    void   ConsumeConsumptionReservation();

    double AddFireReservation    ( double rNbr );
    void   ConsumeFireReservation();

    double GetValue() const;
    double Consume ( double rValue );
    void Reset();

    void   ReadValue( xml::xistream& xis );
    void   Resupply ( double rFactor = 1. );
    void   ChangeDotation( unsigned int number, float threshold );

    bool   HasReachedSupplyThreshold() const;
    bool   NeedSupply               () const;
    double Supply                   ( double rValue );
    void   ChangeValueUsingTC2      ( double rCapacityFactor, MIL_AutomateLOG& tc2 );

    void NotifyCaptured(); //$$$ a renommer et abstraire
    void NotifyReleased();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory() const;
    double GetSupplyThresholdPercentage() const;
    //@}

private:
    //! @name Tools
    //@{
    void SetValue( double rValue );
    //@}

private:
    //! @name Member data
    //@{
    static const double maxCapacity_;
    const PHY_DotationCategory* pCategory_;
    PHY_DotationGroup* pGroup_;
    double rValue_;
    double rCapacity_;
    double rConsumptionReservation_;
    double rFireReservation_;
    double rSupplyThreshold_;
    bool bDotationBlocked_; // Prisoner
    bool bInfiniteDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_Dotation )

#endif // __PHY_Dotation_h_
