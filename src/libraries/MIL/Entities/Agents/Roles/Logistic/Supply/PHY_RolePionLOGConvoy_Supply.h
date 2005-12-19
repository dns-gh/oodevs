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

#include "MIL.h"

#include "PHY_RolePion_Supply.h"

class MIL_AgentPion;
class PHY_StockConvoy;

// =============================================================================
// @class  PHY_RolePionLOGConvoy_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePionLOGConvoy_Supply : public PHY_RolePion_Supply
{
    MT_COPYNOTALLOWED( PHY_RolePionLOGConvoy_Supply )

public:
             PHY_RolePionLOGConvoy_Supply( MT_RoleContainer& role, const MIL_AgentPion& pion );
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
    virtual       void               AssignConvoy           ( PHY_StockConvoy& convoy );
    virtual       void               UnassignConvoy         ( PHY_StockConvoy& convoy );

    virtual       bool               ConvoyLoad             () const;
    virtual       bool               ConvoyUnload           () const;
    virtual       bool               ConvoyIsLoadingDone    () const;
    virtual       bool               ConvoyIsUnloadingDone  () const;
    virtual       const MT_Vector2D* ConvoyGetFormingPoint  () const; 
    virtual       const MT_Vector2D* ConvoyGetLoadingPoint  () const; 
    virtual       const MT_Vector2D* ConvoyGetUnloadingPoint() const; 
    virtual       void               ConvoyEndMission       ();
    //@}

    //! @name Events
    //@{
    virtual void NotifyComposanteChanged( PHY_ComposantePion& composante );
    //@}

private:
    const MIL_AgentPion*   pPion_;
          PHY_StockConvoy* pConvoy_;
};

#include "PHY_RolePionLOGConvoy_Supply.inl"

#endif // __PHY_RolePionLOGConvoy_Supply_h_
