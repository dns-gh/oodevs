// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyStockState.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:34 $
// $Revision: 3 $
// $Workfile: PHY_SupplyStockState.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyStockState_h_
#define __PHY_SupplyStockState_h_

#include "MIL.h"

#include "PHY_SupplyState_ABC.h"
#include "PHY_SupplyStockRequest.h"

class MIL_AutomateLOG;
class PHY_SupplyConsign_ABC;
class PHY_RolePion_Supply;

// =============================================================================
// @class  PHY_SupplyStockState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyStockState : public PHY_SupplyState_ABC
{
    MT_COPYNOTALLOWED( PHY_SupplyStockState )

public:
     PHY_SupplyStockState( MIL_AutomateLOG& suppliedAutomate );
     PHY_SupplyStockState();
    ~PHY_SupplyStockState();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
          void             AddRequest         ( const PHY_SupplyStockRequest& request );
          MT_Float         GetUVolumeRequested() const;
    const MIL_AutomateLOG& GetSuppliedAutomate() const;
          bool             IsSupplying        ( const PHY_DotationCategory& dotationCategory ) const;
          void             RemoveConvoyedStock( MT_Float rUVolumeRemoved );

          void             Supply      () const;
          void             CancelSupply();
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_SupplyConsign_ABC* pConsign );
    //@}

    //! @name Network
    //@{
    void SendMsgCreation () const;
    void SendChangedState() const;
    void SendFullState   () const;
    void Clean           ();
    //@}

private:
    //! @name Network tools
    //@{
    void SendMsgDestruction() const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyStockRequest > T_RequestMap;
    typedef T_RequestMap::iterator                                          IT_RequestMap;
    typedef T_RequestMap::const_iterator                                    CIT_RequestMap;
    //@}

private:
    MIL_AutomateLOG*       pSuppliedAutomate_;
    PHY_SupplyConsign_ABC* pConsign_;
    bool                   bConsignChanged_;
    bool                   bRequestsChanged_;
    T_RequestMap           requests_;
};

#include "PHY_SupplyStockState.inl"

#endif // __PHY_SupplyStockState_h_
