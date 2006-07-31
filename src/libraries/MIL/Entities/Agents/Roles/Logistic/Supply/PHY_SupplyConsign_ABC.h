// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyConsign_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 17:20 $
// $Revision: 5 $
// $Workfile: PHY_SupplyConsign_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyConsign_ABC_h_
#define __PHY_SupplyConsign_ABC_h_

#include "MIL.h"

class NET_ASN_MsgLogRavitaillementTraitementUpdate;
class MIL_AutomateLOG;
class MIL_Automate;

// =============================================================================
// @class  PHY_SupplyConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_SupplyConsign_ABC )

public:
             PHY_SupplyConsign_ABC( MIL_AutomateLOG& supplyingAutomate, const MIL_Automate& suppliedAutomate, MIL_AutomateLOG& convoyingAutomate );
             PHY_SupplyConsign_ABC();
    virtual ~PHY_SupplyConsign_ABC();

    //! @name CheckPoints 
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual bool Update() = 0;

            void Clean     ();
            bool HasChanged() const;

    //@}

    //! @name Accessors
    //@{
          MIL_AutomateLOG& GetSupplyingAutomate() const;
          MIL_AutomateLOG& GetConvoyingAutomate() const;
    const MIL_Automate&    GetSuppliedAutomate () const;

    virtual void GetMerchandiseToConvoy              ( T_MerchandiseToConvoyMap& container ) const = 0;
    virtual void RemoveConvoyedMerchandise           ( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations ) = 0;
    virtual void AddConvoyedMerchandise              ( const PHY_DotationCategory& dotationCategory, MT_Float rNbrDotations ) = 0;
    virtual void CancelMerchandiseOverheadReservation() = 0;
    //@}

    //! @name Network
    //@{
            void SendChangedState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn ) const;
    virtual void SendFullState   ( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn ) const;
    static  void SendDefaultState( NET_ASN_MsgLogRavitaillementTraitementUpdate& asn );
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eConvoyWaitingForTransporters,
        eConvoyForming,
        eConvoyGoingToLoadingPoint,
        eConvoyLoading,
        eConvoyGoingToUnloadingPoint,
        eConvoyUnloading,
        eConvoyGoingBackToFormingPoint,
        eFinished
    };
    //@}

protected:
    //! @name
    //@{
    E_State GetState() const;
    void    SetState( E_State nNewState );    
    //@} 

private:
          MIL_AutomateLOG* pSupplyingAutomate_;
          MIL_AutomateLOG* pConvoyingAutomate_;
    const MIL_Automate*    pSuppliedAutomate_;
          E_State          nState_;
          bool             bHasChanged_;

protected:
    int              nTimer_;
};

#include "PHY_SupplyConsign_ABC.inl"

#endif // __PHY_SupplyConsign_ABC_h_
