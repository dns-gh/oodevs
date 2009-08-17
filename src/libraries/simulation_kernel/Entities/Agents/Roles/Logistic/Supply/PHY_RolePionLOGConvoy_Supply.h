// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOGConvoy_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:26 $
// $Revision: 2 $
// $Workfile: PHY_RolePionLOGConvoy_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RolePionLOGConvoy_Supply_h_
#define __PHY_RolePionLOGConvoy_Supply_h_

#include "PHY_RoleInterface_Supply.h"

class MIL_AgentPion;
class PHY_StockConvoy;

// =============================================================================
// @class  PHY_RolePionLOGConvoy_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOGConvoy_Supply : public PHY_RoleInterface_Supply
{

public:
             PHY_RolePionLOGConvoy_Supply( MIL_AgentPion& pion );
             PHY_RolePionLOGConvoy_Supply();
    virtual ~PHY_RolePionLOGConvoy_Supply();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean ();
    //@}

    //! @name Convoy
    //@{
    virtual       void             AssignConvoy              ( PHY_StockConvoy& convoy );
    virtual       void             UnassignConvoy            ( PHY_StockConvoy& convoy );

    virtual       bool             ConvoyLoad                () const;
    virtual       bool             ConvoyUnload              () const;
    virtual       bool             ConvoyIsLoadingDone       () const;
    virtual       bool             ConvoyIsUnloadingDone     () const;
    virtual const MIL_AutomateLOG* ConvoyGetSupplyingAutomate() const;
    virtual const MIL_AutomateLOG* ConvoyGetConvoyingAutomate() const;
    virtual const MIL_Automate*    ConvoyGetSuppliedAutomate () const;
    virtual       void             ConvoyEndMission          ();

    virtual       MT_Float         ModifySpeed               ( MT_Float rSpeed ) const;
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante );
    //@}

private:
    MIL_AgentPion*   pPion_;
          PHY_StockConvoy* pConvoy_;
};

#endif // __PHY_RolePionLOGConvoy_Supply_h_
