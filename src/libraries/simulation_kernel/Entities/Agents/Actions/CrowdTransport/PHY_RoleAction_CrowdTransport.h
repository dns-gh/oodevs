// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleAction_CrowdTransport_h_
#define __PHY_RoleAction_CrowdTransport_h_

#include "MIL.h"
#include "TransportNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/Role_ABC.h"
#include "Entities/Populations/MIL_PopulationHumans.h"

class MIL_AgentPion;
class MIL_Population;
class MT_Vector2D;

namespace client
{
    class UnitAttributes;
}

namespace crowdtransport
{
// =============================================================================
/** @class  PHY_RoleAction_CrowdTransport
    @brief  PHY_RoleAction_CrowdTransport
*/
// Created: JSR 2011-08-09
// =============================================================================
class PHY_RoleAction_CrowdTransport : public tools::Role_ABC
                                    , public transport::TransportNotificationHandler_ABC
                                    , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_CrowdTransport RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleAction_CrowdTransport( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_CrowdTransport();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    bool IsTransportingCrowd() const;
    //@}

    //! @name Event handler
    //@{
    virtual void NotifyComposanteChanged( const PHY_ComposantePion& composante );
    //@}

    //! @name Action
    //@{
    int  LoadCrowd( MIL_Population& crowd, unsigned int concentrationId );
    void LoadSuspended();
    int  UnloadCrowd( MIL_Population& crowd, const MT_Vector2D& position );
    void UnloadSuspended();
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,
        eFinished,
        eImpossible
    };
    enum E_State
    {
        eNothing,
        eLoading,
        eUnloading
    };
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_CrowdTransport )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&       owner_;
    bool                 bUpdated_;
    E_State              nState_;
    double               currentProgress_;
    MIL_PopulationHumans loadedHumans_;
    MIL_Population*      currentCrowd_;
    bool                 bHasChanged_;
    //@}
};

} // namespace crowdtransport

BOOST_CLASS_EXPORT_KEY( crowdtransport::PHY_RoleAction_CrowdTransport )
namespace crowdtransport
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_CrowdTransport, MIL_AgentPion )
}

#endif // __PHY_RoleAction_CrowdTransport_h_
