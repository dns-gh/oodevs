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
#include "ENT/ENT_Enums.h"
#include <tools/Map.h>

namespace xml
{
    class xostream;
    class xistream;
}

class PHY_DotationCategory;
class PHY_Dotation;
class PHY_DotationType;
class PHY_DotationGroupContainer;
class PHY_DotationCapacity;
class PHY_AmmoDotationClass;
class PHY_UnitType;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_DotationGroup
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationGroup : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, PHY_Dotation* > T_DotationMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_DotationGroup( const PHY_DotationType& type, PHY_DotationGroupContainer& groupContainer, bool bInfiniteDotations );
             PHY_DotationGroup();
    virtual ~PHY_DotationGroup();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    PHY_Dotation& ReadValues( xml::xistream& xis, const PHY_DotationCategory& category ); //$$$ A VIRER
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

    const PHY_DotationCategory* GetIlluminationDotations( float range, bool permanent ) const;
    float GetIlluminatingRange( ) const;

    void Resupply( double rFactor = 1., bool withLog = false );
    void Resupply( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor );
    //@}

    //! @name Misc
    //@{
    void NotifyCaptured();
    void NotifyReleased();
    //@}

    //! @name Notifications
    //@{
    void NotifyDotationChanged( const PHY_Dotation& dotation );
    //@}

    //! @name Logistic - supply
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void Apply                       ( boost::function< void( PHY_Dotation& ) > visitor ) const;
    void ChangeDotationsValueUsingTC2( const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor, MIL_AutomateLOG& tc2 ) const;
    void EnforceAviationResources    ( E_AviationRange aviationRange, const PHY_UnitType& unitType, MIL_AutomateLOG& tc2 );
    //@}

    //! @name Accessors
    //@{
    const T_DotationMap& GetDotations() const;
    PHY_Dotation* GetDotation( const PHY_DotationCategory& dotationCategory ) const;
    const PHY_DotationGroupContainer* GetDotationGroupContainer() const;
    //@}

private:
    //! @name Tools
    //@{
    PHY_Dotation& CreateDotation( const PHY_DotationCategory& dotationCategory );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationType* pType_;
    PHY_DotationGroupContainer* pGroupContainer_;
    T_DotationMap dotations_;
    bool bInfiniteDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_DotationGroup )

#endif // __PHY_DotationGroup_h_
