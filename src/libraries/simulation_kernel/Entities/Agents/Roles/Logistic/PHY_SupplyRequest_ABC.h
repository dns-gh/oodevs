// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyRequest_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:23 $
// $Revision: 3 $
// $Workfile: PHY_SupplyRequest_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyRequest_ABC_h_
#define __PHY_SupplyRequest_ABC_h_

#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "protocol/SimulationSenders.h"

class MIL_AgentPion;

class PHY_DotationCategory;

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
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool AffectAutomate( MIL_AutomateLOG& supplyingAutomate );
    bool HasReachedSupplyThreshold() const;
    void ReserveStocks();

    double Supply() const;
    void Cancel();

    void RemoveConvoyedMerchandise( double rNbr );
    void AddConvoyedMerchandise( double rNbr );
    void CancelMerchandiseOverheadReservation();
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetDotationCategory   () const;
          MIL_AutomateLOG*      GetSupplyingAutomate  () const;
          MIL_Automate&         GetStockPion          () const;
          double              GetTotalRequestedValue() const;
          double              GetTotalReservedValue () const;
          double              GetTotalConvoyedValue () const;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::DotationQuery& asn ) const;
    //@}

protected:
    //! @name Types
    //@{
    struct sIndividualRequest
    {
        T*          pRequest_;       // == le pion demandeur PHY_Dotation / PHY_DotationStock
        double    rRequestedValue_;

        template< typename Archive > void serialize( Archive&, const unsigned int );
    };
    typedef typename std::vector< sIndividualRequest >    T_RequestVector;
    typedef typename T_RequestVector::iterator            IT_RequestVector;
    typedef typename T_RequestVector::const_iterator      CIT_RequestVector;
    //@}

protected:
    T_RequestVector      requests_;
    MIL_AgentPion*       pStockPion_;
    double             rTotalRequestedValue_;
    MIL_AutomateLOG*     pSupplyingAutomate_;
    double             rTotalReservedValue_;
    double             rTotalConvoyedValue_;
};

#include "PHY_SupplyRequest_ABC.inl"

#endif // __PHY_SupplyRequest_ABC_h_
