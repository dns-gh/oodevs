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

#include "MIL.h"

namespace xml
{
    class xistream;
    class xostream;
}

class PHY_DotationCategory;
class PHY_DotationCapacity;
class PHY_DotationStock;
class PHY_RolePionLOG_Supply;
class NET_ASN_MsgLogSupplyState;
class PHY_SupplyStockRequestContainer;

// =============================================================================
// @class  PHY_DotationStockContainer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationStockContainer
{
    MT_COPYNOTALLOWED( PHY_DotationStockContainer )

public:
     PHY_DotationStockContainer( PHY_RolePionLOG_Supply& roleSupply );
     PHY_DotationStockContainer();
    ~PHY_DotationStockContainer();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
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
    MT_Float AddReservation   ( const PHY_DotationCategory& category, MT_Float rNbr );
    void     RemoveReservation( const PHY_DotationCategory& category, MT_Float rNbr );
    void     Resupply         ();
    void     Resupply         ( const PHY_DotationCategory& category, MT_Float rNbr );
    //@}

    //! @name Accessors
    //@{
    MT_Float           GetValue( const PHY_DotationCategory& category ) const;
    PHY_DotationStock* GetStock( const PHY_DotationCategory& category ) const;
    PHY_DotationStock* AddStock( const PHY_DotationCategory& category );
    //@}

    //! @name Network
    //@{
    void NotifyDotationChanged( const PHY_DotationStock& dotationStock, MT_Float rDelta );
    bool HasChanged           () const;
    void SendChangedState     ( NET_ASN_MsgLogSupplyState& asn ) const;
    void SendFullState        ( NET_ASN_MsgLogSupplyState& asn ) const;
    //@}

    //! @name Supply
    //@{
    void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void FillSupplyRequest ( PHY_SupplyStockRequestContainer& supplyRequest ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadStock( xml::xistream& xis );
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_DotationStock* > T_StockMap;
    typedef T_StockMap::const_iterator                                  CIT_StockMap;

    typedef std::set< const PHY_DotationStock* > T_StockSet;
    typedef T_StockSet::const_iterator           CIT_StockSet;
    //@}

private:
    //! @name Tools
    //@{
    PHY_DotationStock* AddStock            ( const PHY_DotationCategory& category, xml::xistream& xis);
    void               CheckStockCapacities();
    //@}

private:
    PHY_RolePionLOG_Supply* pRoleSupply_;
    T_StockMap              stocks_;   
    T_StockSet              stocksChanged_;
    bool                    bCheckStockCapacities_;
};

#include "PHY_DotationStockContainer.inl"

#endif // __PHY_DotationStockContainer_h_
