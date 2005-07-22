// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockRequest.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:23 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockRequest.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockRequest_h_
#define __PHY_SupplyStockRequest_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_DotationStock;
class MIL_AutomateLOG;
class PHY_RolePion_Supply;

struct ASN1T_DemandeDotation;

// =============================================================================
// @class  PHY_SupplyStockRequest
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockRequest
{
public:
     PHY_SupplyStockRequest();
    ~PHY_SupplyStockRequest();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void     ApplyQuota               ( MT_Float rQuota );
    void     AddStock                 ( PHY_DotationStock& stock );
    void     AddStockPushedFlow       ( PHY_DotationStock& stock );
    void     SetStockPushedFlowValue  ( MT_Float rValue );
    bool     AffectAutomate           ( MIL_AutomateLOG& supplyingAutomate );
    bool     HasReachedSupplyThreshold() const;
    void     ReserveStocks            ();

    MT_Float Supply                   () const;
    void     Cancel                   ();

    void     RemoveConvoyedStock      ( MT_Float rNbrToRemove );
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetDotationCategory   () const;
          MIL_AutomateLOG*      GetSupplyingAutomate  () const;
          MT_Float              GetTotalRequestedValue() const;
          MT_Float              GetTotalReservedValue () const;
    //@}

    //! @name Network
    //@{
    void Serialize( ASN1T_DemandeDotation& asn ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sStockRequest
    {
        PHY_DotationStock* pStock_;       // == le pion demandeur
        MT_Float           rRequestedValue_;
        
        template< typename Archive > void serialize( Archive&, const uint );
    };
    typedef std::vector< sStockRequest >    T_RequestVector;
    typedef T_RequestVector::iterator       IT_RequestVector;
    typedef T_RequestVector::const_iterator CIT_RequestVector;
    //@}

private:
    T_RequestVector      requests_;
    MT_Float             rTotalRequestedValue_;   
    MIL_AutomateLOG*     pSupplyingAutomate_;
    PHY_RolePion_Supply* pStockPion_;
    MT_Float             rTotalReservedValue_;   
    MT_Float             rTotalConvoyedValue_;
};

#include "PHY_SupplyStockRequest.inl"

#endif // __PHY_SupplyStockRequest_h_
