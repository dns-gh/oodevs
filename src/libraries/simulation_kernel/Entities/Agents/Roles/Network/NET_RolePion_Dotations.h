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

#include "NET_RoleInterface_Dotations.h"
#include "game_asn/Simulation.h"
#include "ComponentsChangedNotificationHandler_ABC.h"

class PHY_ComposanteTypePion;
class PHY_Dotation;
class MIL_AgentPion;
class NET_ASN_MsgUnitDotations;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  NET_RolePion_Dotations
// $$$$ SBO 2009-06-30: WTF? name doesn't match anything the class actually does
// Created: JVT 2004-08-03
// =============================================================================
class NET_RolePion_Dotations : public NET_RoleInterface_Dotations
                             , public component::ComponentsChangedNotificationHandler_ABC
{

public:
    explicit NET_RolePion_Dotations( MIL_AgentPion& pion );
    virtual ~NET_RolePion_Dotations();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    void Clean();
    //@}

    //! @name Notification
    //@{
    virtual void NotifyHasChanged();
    //@}

private:
    //! @name Tools
    //@{
    bool DataUpdated() const;
    void SendMsg    ( NET_ASN_MsgUnitAttributes& asnMsg ) const;
    //@}

private:
    MIL_AgentPion* pPion_;
    mutable bool   bLastStateDead_;
    mutable bool   bLastStateNeutralized_;
    bool bExternalMustUpdateData_;
    bool bExternalMustUpdateVisionCones_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const NET_RolePion_Dotations* role, const unsigned int /*version*/ );
  	template< typename Archive > friend  void load_construct_data( Archive& archive, NET_RolePion_Dotations* role, const unsigned int /*version*/ );

};


#endif // __NET_RolePion_Dotations_h_
