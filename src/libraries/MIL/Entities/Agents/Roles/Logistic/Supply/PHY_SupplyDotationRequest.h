// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationRequest.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:20 $
// $Revision: 2 $
// $Workfile: PHY_SupplyDotationRequest.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationRequest_h_
#define __PHY_SupplyDotationRequest_h_

#include "MIL.h"

class PHY_DotationCategory;
class PHY_Dotation;
class PHY_RolePion_Supply;
class MIL_AutomateLOG;

struct ASN1T_DemandeDotation;

// =============================================================================
// @class  PHY_SupplyDotationRequest
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationRequest
{
public:
     PHY_SupplyDotationRequest();
    ~PHY_SupplyDotationRequest();

    //! @name 
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void     AddDotation              ( PHY_Dotation& dotation );
    MT_Float Supply                   () const;
    bool     AffectAutomate           ( MIL_AutomateLOG& supplyingAutomate );
    bool     HasReachedSupplyThreshold() const;
    void     ReserveStocks            ();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetDotationCategory  () const;
          MIL_AutomateLOG*      GetSupplyingAutomate () const;
          MT_Float              GetTotalReservedValue() const;          
    //@}

    //! @name Network
    //@{
    void Serialize( ASN1T_DemandeDotation& asn ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sDotationRequest
    {
        PHY_Dotation* pDotation_;       // == le pion demandeur
        MT_Float      rRequestedValue_;
    
        template< typename Archive > void serialize( Archive&, const uint );
    };
    
    typedef std::vector< sDotationRequest > T_RequestVector;
    typedef T_RequestVector::const_iterator CIT_RequestVector;
    //@}

private:
    T_RequestVector      requests_;
    MT_Float             rTotalRequestedValue_;   
    MIL_AutomateLOG*     pSupplyingAutomate_;
    PHY_RolePion_Supply* pStockPion_;
    MT_Float             rTotalReservedValue_;   
};

#include "PHY_SupplyDotationRequest.inl"

#endif // __PHY_SupplyDotationRequest_h_
