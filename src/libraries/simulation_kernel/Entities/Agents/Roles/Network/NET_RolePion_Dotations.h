// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RolePion_Dotations.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:40 $
// $Revision: 4 $
// $Workfile: NET_RolePion_Dotations.h $
//
// *****************************************************************************

#ifndef __NET_RolePion_Dotations_h_
#define __NET_RolePion_Dotations_h_

#include "MIL.h"

#include "NET_RoleInterface_Dotations.h"
#include "game_asn/Simulation.h"

class PHY_ComposanteTypePion;
class PHY_Dotation;
class MIL_AgentPion;
class NET_ASN_MsgUnitDotations;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  NET_RolePion_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class NET_RolePion_Dotations : public NET_RoleInterface_Dotations
{
    MT_COPYNOTALLOWED( NET_RolePion_Dotations )

public:
             NET_RolePion_Dotations( MT_RoleContainer& role, const MIL_AgentPion& pion );
             NET_RolePion_Dotations();
    virtual ~NET_RolePion_Dotations();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    //@}

private:
    //! @name Tools
    //@{
    bool DataUpdated() const;
    void SendMsg    ( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    //@}

private:
    const MIL_AgentPion* pPion_;
    mutable bool         bLastStateDead_;
    mutable bool         bLastStateNeutralized_;
};

#include "NET_RolePion_Dotations.inl"

#endif // __NET_RolePion_Dotations_h_
