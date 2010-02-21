// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 7 $
// $Workfile: PHY_DotationGroupContainer.h $
//
// *****************************************************************************

#ifndef __PHY_DotationGroupContainer_h_
#define __PHY_DotationGroupContainer_h_

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

class PHY_DotationType;
class PHY_DotationGroup;
class PHY_DotationCategory;
class PHY_DotationCapacity;
class PHY_Dotation;
class PHY_SupplyDotationRequestContainer;
class MIL_AutomateLOG;
class PHY_AmmoDotationClass;

namespace dotation
{
    class PHY_RoleInterface_Dotations;
}
// =============================================================================
// @class  PHY_DotationGroupContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationGroupContainer : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_DotationGroupContainer( dotation::PHY_RoleInterface_Dotations& roleDotation );
             PHY_DotationGroupContainer();
            ~PHY_DotationGroupContainer();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}
    
    //! @name Init
    //@{
    void ReadValues( xml::xistream& xis );
    void WriteODB  ( xml::xostream& xos ) const;
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
    void     Resupply( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor );
    void     Resupply( const PHY_DotationType&     type              , MT_Float rFactor );
    const PHY_DotationCategory* GetIlluminationDotations( float range, bool permanent ) const;
    //@}

    //! @name 
    //@{
    void NotifyCaptured();
    void NotifyReleased();
    //@}

    //! @name Logistic
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& supplyRequest ) const;
    void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor, MIL_AutomateLOG& tc2 ) const;
    //@}

    //! @name Network
    //@{
    void NotifyDotationChanged( const PHY_Dotation& dotation );
    bool HasChanged           () const;
    void Clean                ();
    void SendChangedState     ( client::UnitAttributes& asn ) const;
    void SendFullState        ( client::UnitAttributes& asn ) const;
    //@}

private:
    //! @name Tools
    //@{
    PHY_DotationGroup* GetDotationGroup   ( const PHY_DotationType& dotationType ) const;
    PHY_DotationGroup& CreateDotationGroup( const PHY_DotationType& dotationType );
    //@}
    //! @name Helpers
    //@{
    void ReadDotations( xml::xistream& xis );
    void ReadDotation ( xml::xistream& xis );
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationType*, PHY_DotationGroup* > T_DotationGroupMap;
    typedef T_DotationGroupMap::const_iterator                      CIT_DotationGroupMap;  

    typedef std::set< const PHY_Dotation* > T_DotationSet;
    typedef T_DotationSet::const_iterator   CIT_DotationSet;
    //@}

private:
    //! @name Member data
    //@{
    dotation::PHY_RoleInterface_Dotations* pRoleDotation_;
    T_DotationGroupMap      dotationGroups_;
    T_DotationSet           dotationsChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_DotationGroupContainer )

#endif // __PHY_DotationGroupContainer_h_
