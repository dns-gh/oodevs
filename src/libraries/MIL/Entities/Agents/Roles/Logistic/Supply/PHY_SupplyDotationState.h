// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyDotationState.h $
// $Author: Jvt $
// $Modtime: 11/04/05 13:30 $
// $Revision: 3 $
// $Workfile: PHY_SupplyDotationState.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyDotationState_h_
#define __PHY_SupplyDotationState_h_

#include "MIL.h"

#include "PHY_SupplyState_ABC.h"
#include "PHY_SupplyDotationRequest.h"

class MIL_Automate;
class MIL_AutomateLOG;
class PHY_SupplyConsign_ABC;
class PHY_RolePion_Supply;

// =============================================================================
// @class  PHY_SupplyDotationState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyDotationState : public PHY_SupplyState_ABC
{
    MT_COPYNOTALLOWED( PHY_SupplyDotationState )

public:
     PHY_SupplyDotationState( MIL_Automate& suppliedAutomate );
     PHY_SupplyDotationState();
    ~PHY_SupplyDotationState();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
          void          AddRequest         ( const PHY_SupplyDotationRequest& request );
          void          GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const;
    const MIL_Automate& GetSuppliedAutomate() const;
          bool          IsSupplying        ( const PHY_DotationCategory& dotationCategory ) const;

          void          Supply             () const;
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
    typedef std::map< const PHY_DotationCategory*, PHY_SupplyDotationRequest > T_RequestMap;
    typedef T_RequestMap::const_iterator                                       CIT_RequestMap;
    //@}

private:
    MIL_Automate*          pSuppliedAutomate_;
    PHY_SupplyConsign_ABC* pConsign_;
    bool                   bHasChanged_;
    T_RequestMap           requests_;
};

#include "PHY_SupplyDotationState.inl"

#endif // __PHY_SupplyDotationState_h_
