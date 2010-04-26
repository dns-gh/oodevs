// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h $
// $Author: Nld $
// $Modtime: 18/04/05 16:40 $
// $Revision: 4 $
// $Workfile: PHY_RoleInterface_Dotations.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Dotations_h_
#define __PHY_RoleInterface_Dotations_h_

#include "MT_Tools/Role_ABC.h"
#include <xeumeuleu/xml.h>
#include <boost/serialization/serialization.hpp>

namespace client
{
    class UnitAttributes;
}

class PHY_DotationCategory;
class PHY_DotationType;
class PHY_Dotation;
class PHY_ConsumptionType;
class PHY_SupplyDotationRequestContainer;
class PHY_AmmoDotationClass;

namespace dotation
{

// =============================================================================
// @class  PHY_RoleInterface_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Dotations : public tools::Role_ABC
                                  , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Dotations RoleInterface;
    //@}
    
public:
    //! @name Constructor/Destructor
    //@{
             PHY_RoleInterface_Dotations();
    virtual ~PHY_RoleInterface_Dotations();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetConsumptionMode() const = 0;
    //@}

    virtual void ReadOverloading              ( xml::xistream& xis ) = 0;
    //@}

    //! @name Operations 
    //@{
    virtual void Update    ( bool bIsDead ) = 0;
    virtual void Clean     () = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Dotations management
    //@{
    virtual double SupplyDotation     ( const PHY_DotationCategory& category, double rNbr ) = 0;
    virtual void   ResupplyDotations  () = 0;
    virtual void   ResupplyDotations  ( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor ) = 0;
    virtual void   ResupplyDotations  ( const PHY_DotationType&      type             , double rFactor ) = 0;
    //@}

    //! @name Consumption management
    //@{
    virtual bool                 SetConsumptionMode      ( const PHY_ConsumptionType& mode ) = 0;
    virtual void                 RollbackConsumptionMode () = 0;
    virtual double             GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const = 0;
    //@}

    //! @name Fire dotations
    //@{
    virtual double AddFireReservation( const PHY_DotationCategory& category, double rNbr ) = 0;
    virtual double GetDotationNumber( const PHY_DotationCategory& category ) const = 0;
    virtual const PHY_DotationCategory* GetIlluminationDotations( float range, bool permanent ) const = 0;
    virtual void SetForbiddenDotation( const PHY_DotationCategory& category ) = 0;
    virtual void RemoveForbiddenDotation( const PHY_DotationCategory& category ) = 0;
    virtual void AllowAllDotations() = 0;

    //@}

    //! @name Network
    //@{
//    virtual void SendChangedState( client::UnitAttributes& asn ) const = 0;
//    virtual void SendFullState   ( client::UnitAttributes& asn ) const = 0;
    //@}

    //! @name Logistic - Supply
    //@{
    virtual void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const = 0; // Logistic
    virtual void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& supplyRequest ) const = 0;
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const = 0;
    //@}
private:
    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const uint )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}
};

} // namespace dotation

#endif // __PHY_RoleInterface_Dotations_h_
