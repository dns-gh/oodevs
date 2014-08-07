// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationStockContainer.h $
// $Author: Jvt $
// $Modtime: 31/03/05 15:51 $
// $Revision: 3 $
// $Workfile: PHY_DotationStockContainer.h $
//
// *****************************************************************************

#ifndef __PHY_DotationStockContainer_h_
#define __PHY_DotationStockContainer_h_

#include <tools/Map.h>
#include <tools/Set.h>
#include <boost/serialization/export.hpp>

namespace client
{
    class LogSupplyState;
}

namespace xml
{
    class xistream;
    class xostream;
}

class PHY_DotationCategory;
class PHY_DotationStock;
class PHY_RoleInterface_Supply;
class PHY_DotationNature;

// =============================================================================
// @class  PHY_DotationStockContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationStockContainer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_DotationStockContainer();
             PHY_DotationStockContainer( PHY_RoleInterface_Supply& roleSupply, bool bInfiniteDotations );
    virtual ~PHY_DotationStockContainer();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    void ReadValues( xml::xistream& xis);
    //@}

    //! @name Main
    //@{
    void Clean ();
    void Update();
    //@}

    //! @name Operations
    //@{
    double AddReservation   ( const PHY_DotationCategory& category, double rNbr ); // Return the quantity reserved
    double RemoveReservation( const PHY_DotationCategory& category, double rNbr ); // Return the quantity returned
    void Resupply( bool withLog );
    void Resupply( const PHY_DotationCategory& category, double rNbr );
    void ComputeStockWeightAndVolume( const PHY_DotationNature&, double& rWeight, double& rVolume ) const;
    //@}

    //! @name Accessors
    //@{
    double GetValue( const PHY_DotationCategory& category ) const;
    PHY_DotationStock* GetStock( const PHY_DotationCategory& category ) const;
    PHY_DotationStock* AddStock( const PHY_DotationCategory& category );
    PHY_DotationStock* AddEmptyStock( const PHY_DotationCategory& category, double capacity );
    const PHY_RoleInterface_Supply* GetRoleInterfaceSupply() const;
    //@}

    //! @name Network
    //@{
    void NotifyDotationChanged( const PHY_DotationStock& dotationStock, double rDelta );
    bool HasChanged() const;
    void SendChangedState( client::LogSupplyState& asn ) const;
    void SendFullState( client::LogSupplyState& asn ) const;
    //@}

    //! @name Supply
    //@{
    void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void Apply             ( std::function< void( PHY_DotationStock& ) > visitor ) const;
    void UpdateSupplyNeeded();
    //@}

    //! @name Helpers
    //@{
    void ReadStock( xml::xistream& xis );
    //@}

public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, PHY_DotationStock* > T_StockMap;
    //@}

private:
    typedef tools::Set< const PHY_DotationStock* > T_StockSet;

private:
    //! @name Tools
    //@{
    PHY_DotationStock* AddStock( const PHY_DotationCategory& category, xml::xistream& xis);
    void CheckStockCapacities();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleInterface_Supply* pRoleSupply_;
    T_StockMap stocks_;
    T_StockSet stocksChanged_;
    bool bCheckStockCapacities_;
    bool bInfiniteDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_DotationStockContainer )

#endif // __PHY_DotationStockContainer_h_
