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
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"

namespace client
{
    class UnitAttributes;
}

class MIL_AgentPion;

namespace network
{

// =============================================================================
// @class  NET_RolePion_Dotations
// $$$$ SBO 2009-06-30: WTF? name doesn't match anything the class actually does
// Created: JVT 2004-08-03
// =============================================================================
class NET_RolePion_Dotations : public NET_RoleInterface_Dotations
                             , public NetworkNotificationHandler_ABC
{
public:
    explicit NET_RolePion_Dotations( MIL_AgentPion& pion );
    virtual ~NET_RolePion_Dotations();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Network
    //@{
    void SendChangedState( unsigned int context = 0 ) const;
    void SendFullState   ( unsigned int context ) const;
    void Clean();
    //@}

    //! @name Notification
    //@{
    virtual void NotifyDataHasChanged();
    //@}

private:
    //! @name Tools
    //@{
    void SendMsgUnitAttributes( client::UnitAttributes& asnMsg, unsigned int context ) const;
    //@}

private:
    MIL_AgentPion& pion_;
    mutable bool   bLastStateDead_;
    mutable bool   bLastStateNeutralized_;
    bool bExternalMustUpdateData_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const NET_RolePion_Dotations* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, NET_RolePion_Dotations* role, const unsigned int /*version*/ );

};

} // namespace network

BOOST_CLASS_EXPORT_KEY( network::NET_RolePion_Dotations )

#endif // __NET_RolePion_Dotations_h_
