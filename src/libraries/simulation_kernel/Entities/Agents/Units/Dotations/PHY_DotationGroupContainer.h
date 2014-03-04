// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __PHY_DotationGroupContainer_h_
#define __PHY_DotationGroupContainer_h_

#include "ENT/ENT_Enums.h"
#include <tools/Map.h>
#include <tools/Set.h>
#include <boost/serialization/export.hpp>

namespace client
{
    class UnitAttributes;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace dotation
{
    class PHY_RoleInterface_Dotations;
}

class PHY_DotationType;
class PHY_DotationGroup;
class PHY_DotationCategory;
class PHY_DotationCapacity;
class PHY_Dotation;
class MIL_AutomateLOG;
class PHY_AmmoDotationClass;
class PHY_UnitType;
class MIL_CheckPointOutArchive;
class MIL_CheckPointInArchive;

// =============================================================================
// @class  PHY_DotationGroupContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationGroupContainer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_DotationGroupContainer( dotation::PHY_RoleInterface_Dotations& roleDotation, bool bInfiniteDotations );
             PHY_DotationGroupContainer();
            ~PHY_DotationGroupContainer();
    //@}

    //! @name CheckPoints
    //@{
    void serialize( MIL_CheckPointOutArchive& ar, unsigned int );
    void serialize( MIL_CheckPointInArchive& ar, unsigned int );
    //@}

    //! @name Init
    //@{
    void ReadDotations( xml::xistream& xis, const PHY_UnitType& unitType );
    void WriteODB     ( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void   AddCapacity   ( const PHY_DotationCapacity& capacity, double toAdd );
    double RemoveCapacity( const PHY_DotationCapacity& capacity );
    double GetCapacity   ( const PHY_DotationCategory& category ) const;

    void   CancelConsumptionReservations ();
    double AddConsumptionReservation     ( const PHY_DotationCategory& category, double rNbr );
    void   ConsumeConsumptionReservations();
    void   UpdateSupplyNeeded();

    double AddFireReservation     ( const PHY_DotationCategory& category, double rNbr );
    void   ConsumeFireReservations();

    double GetValue( const PHY_DotationCategory& category ) const;
    double Consume ( const PHY_DotationCategory& category, double rNbr );
    double Supply  ( const PHY_DotationCategory& category, double rNbr );
    void   Resupply( bool withLog );
    void   Resupply( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor );
    void   Resupply( const PHY_DotationType&     type              , double rFactor );
    void   ChangeDotation( const PHY_DotationCategory& category, unsigned int number, float thresholdPercentage );
    float  GetIlluminatingRange() const;
    const PHY_DotationCategory* GetIlluminationDotations( float range, bool permanent ) const;
    double GetFuelDotationNumber() const;
    //@}

    //! @name Accessors
    //@{
    const dotation::PHY_RoleInterface_Dotations* GetRoleDotations() const;
    //@}

    //! @name
    //@{
    void NotifyCaptured();
    void NotifyReleased();
    //@}

    //! @name Logistic
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void Apply                       ( boost::function< void( PHY_Dotation& ) > visitor ) const;
    void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor, MIL_AutomateLOG& tc2 ) const;
    void EnforceAviationResources    ( E_AviationRange aviationRange, const PHY_UnitType& unitType, MIL_AutomateLOG& tc2 );
    //@}

    //! @name Network
    //@{
    void NotifyDotationChanged( const PHY_Dotation& dotation );
    bool HasChanged           () const;
    void Clean                ();
    void SendChangedState     ( client::UnitAttributes& asn ) const;
    void SendFullState        ( client::UnitAttributes& asn ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationType*, PHY_DotationGroup* > T_DotationGroupMap;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Set< const PHY_Dotation* > T_DotationSet;
    //@}

private:
    //! @name Tools
    //@{
    PHY_DotationGroup* GetDotationGroup   ( const PHY_DotationType& dotationType ) const;
    PHY_DotationGroup& CreateDotationGroup( const PHY_DotationType& dotationType );
    void               PurgeDotationNotOverloaded( PHY_Dotation& dotation, const T_DotationSet& overloadedDotations );
    //@}

    //! @name Helpers
    //@{
    void ReadDotation ( xml::xistream& xis, const PHY_UnitType& unitType, T_DotationSet& overloadedDotations );
    //@}

private:
    //! @name Member data
    //@{
    dotation::PHY_RoleInterface_Dotations* pRoleDotation_;
    T_DotationGroupMap dotationGroups_;
    T_DotationSet dotationsChanged_;
    bool bInfiniteDotations_;
    //@}
};

#endif // __PHY_DotationGroupContainer_h_
