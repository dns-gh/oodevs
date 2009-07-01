// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyRequest_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:23 $
// $Revision: 3 $
// $Workfile: PHY_SupplyRequest_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyRequest_ABC_h_
#define __PHY_SupplyRequest_ABC_h_

#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

class PHY_DotationCategory;
struct ASN1T_DotationQuery;

// =============================================================================
// @class  PHY_SupplyRequest_ABC
// Created: JVT 2004-08-03
// =============================================================================
template< typename T >
class PHY_SupplyRequest_ABC
{
public:
             PHY_SupplyRequest_ABC();
    virtual ~PHY_SupplyRequest_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    bool     AffectAutomate           ( MIL_AutomateLOG& supplyingAutomate );
    bool     HasReachedSupplyThreshold() const;
    void     ReserveStocks            ();

    MT_Float Supply                   () const;
    void     Cancel                   ();

    void     RemoveConvoyedMerchandise           ( MT_Float rNbr );
    void     AddConvoyedMerchandise              ( MT_Float rNbr );
    void     CancelMerchandiseOverheadReservation();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetDotationCategory   () const;
          MIL_AutomateLOG*      GetSupplyingAutomate  () const;
          MT_Float              GetTotalRequestedValue() const;
          MT_Float              GetTotalReservedValue () const;
          MT_Float              GetTotalConvoyedValue () const;
    //@}

    //! @name Network
    //@{
    void Serialize( ASN1T_DotationQuery& asn ) const;
    //@}

protected:
    //! @name Types
    //@{
    struct sIndividualRequest
    {
        T*          pRequest_;       // == le pion demandeur PHY_Dotation / PHY_DotationStock
        MT_Float    rRequestedValue_;
        
        template< typename Archive > void serialize( Archive&, const uint );
    };
    typedef typename std::vector< sIndividualRequest >    T_RequestVector;
    typedef typename T_RequestVector::iterator            IT_RequestVector;
    typedef typename T_RequestVector::const_iterator      CIT_RequestVector;
    //@}

protected:
    T_RequestVector      requests_;    
    PHY_RolePion_Supply* pStockPion_;
    MT_Float             rTotalRequestedValue_;   
    MIL_AutomateLOG*     pSupplyingAutomate_;
    MT_Float             rTotalReservedValue_;   
    MT_Float             rTotalConvoyedValue_;
};

#include "PHY_SupplyRequest_ABC.inl"

#endif // __PHY_SupplyRequest_ABC_h_
