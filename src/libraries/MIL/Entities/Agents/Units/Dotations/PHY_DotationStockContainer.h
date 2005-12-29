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

class PHY_DotationCategory;
class PHY_DotationCapacity;
class PHY_DotationStock;
class PHY_RolePionLOG_Supply;
class NET_ASN_MsgLogRavitaillementEtat;
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
    //@}
    
    //! @name Init
    //@{
    void ReadValues( MIL_InputArchive& archive );
    //@}

    //! @name Operations
    //@{
    MT_Float AddReservation   ( const PHY_DotationCategory& category, MT_Float rNbr );
    void     RemoveReservation( const PHY_DotationCategory& category, MT_Float rNbr );
    void     AddCapacity      ( const PHY_DotationCapacity& capacity );
    void     Resupply         ();
    void     Resupply         ( const PHY_DotationCategory& category, MT_Float rNbr );
    //@}

    //! @name Accessors
    //@{
    MT_Float            GetValue( const PHY_DotationCategory& category ) const;
    PHY_DotationStock*  GetStock( const PHY_DotationCategory& category ) const;
    //@}

    //! @name Network
    //@{
    void NotifyDotationChanged( const PHY_DotationStock& dotationStock );
    bool HasChanged           () const;
    void Clean                ();
    void SendChangedState     ( NET_ASN_MsgLogRavitaillementEtat& asn ) const;
    void SendFullState        ( NET_ASN_MsgLogRavitaillementEtat& asn ) const;
    //@}

    //! @name Supply
    //@{
    void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const;
    void FillSupplyRequest ( PHY_SupplyStockRequestContainer& supplyRequest ) const;
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
    PHY_RolePionLOG_Supply* pRoleSupply_;
    T_StockMap              stocks_;   
    T_StockSet              stocksChanged_;
};

#include "PHY_DotationStockContainer.inl"

#endif // __PHY_DotationStockContainer_h_
