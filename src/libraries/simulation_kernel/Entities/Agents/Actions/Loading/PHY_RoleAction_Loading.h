// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h $
// $Author: Jvt $
// $Modtime: 4/05/05 14:41 $
// $Revision: 5 $
// $Workfile: PHY_RoleAction_Loading.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Loading_h_
#define __PHY_RoleAction_Loading_h_

#include "MIL.h"
#include "TransportNotificationHandler_ABC.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/Role_ABC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

class MIL_Agent_ABC;

namespace client
{
    class UnitAttributes;
}

namespace posture
{
    class PostureComputer_ABC;
}

namespace transport
{

// =============================================================================
// @class  PHY_RoleAction_Loading
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Loading : public tools::Role_ABC
                             , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                             , public transport::TransportNotificationHandler_ABC
                             , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Loading RoleInterface;
    //@}

public:
    explicit PHY_RoleAction_Loading( MIL_Agent_ABC& pion );
    virtual ~PHY_RoleAction_Loading();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    bool HasChanged() const;
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    //@}

    //! @name Operations
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;

    int  Load          ();
    void LoadSuspended ();

    int  Unload         ();
    void UnloadSuspended();

    void ForceUnloadedState();
    //@}

    //! @name Event handler
    //@{
    virtual void LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, bool& bTransportedByAnother );
    virtual void UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable, MT_Vector2D* position );
    virtual void CheckConsistency();
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsLoaded() const;
    bool IsUnloaded() const;
    double GetLoadingTime  () const;
    double GetUnloadingTime() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,
        eEnd,
        eErrorNoCarriers,
        eErrorNoCarried,
        eErrorLoadUnloadInSameTime
    };

    enum E_State
    {
        eLoading,
        eUnloading,
        eNothing
    };
    //@}

private:
    //! @name Tools
    //@{
    double ComputeLoadingTime  () const;
    double ComputeUnloadingTime() const;
    void SetLoadedState  ();
    void SetUnloadedState();
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_Loading )
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC&  owner_;
    E_State         nState_;
    bool            bIsLoaded_;
    unsigned int    nEndTimeStep_; // Load or unload
    bool            bHasChanged_;
    bool            bHasBeenUpdated_; // Le trigger Load / Unload ou RecoverCarriers a été appelé durant le pas de temps
    //@}
};

} // namespace transport

BOOST_CLASS_EXPORT_KEY( transport::PHY_RoleAction_Loading )
namespace transport
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_Loading, MIL_Agent_ABC )
}

#endif // __PHY_RoleAction_Loading_h_
