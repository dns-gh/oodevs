// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h $
// $Author: Nld $
// $Modtime: 22/06/05 17:35 $
// $Revision: 6 $
// $Workfile: PHY_RoleAction_Transport.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Transport_h_
#define __PHY_RoleAction_Transport_h_

#include "MIL.h"
#include "TransportNotificationHandler_ABC.h"
#include "Entities/Agents/Roles/NBC/ToxicEffectHandler_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/Role_ABC.h"

namespace client
{
    class UnitAttributes;
}

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_ToxicEffectManipulator;
class MT_Vector2D;
class PHY_ComposantePion;

namespace transport
{
// =============================================================================
// @class  PHY_RoleAction_Transport
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Transport : public tools::Role_ABC
                               , public nbc::ToxicEffectHandler_ABC
                               , public transport::TransportNotificationHandler_ABC
                               , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Transport RoleInterface;

    struct sTransportData
    {
        sTransportData();
        sTransportData( double rTotalWeight, bool bTransportOnlyLoadable );

        bool     bTransportOnlyLoadable_;
        double rTotalWeight_;
        double rRemainingWeight_;
        double rTransportedWeight_;

        template< class Archive >
        void serialize( Archive & ar, unsigned int /*version*/ )
        {
            ar & bTransportOnlyLoadable_
                & rTotalWeight_
                & rRemainingWeight_
                & rTransportedWeight_;
        }
    };

    typedef std::map< MIL_Agent_ABC*, sTransportData > T_TransportedPionMap;
    typedef T_TransportedPionMap::iterator             IT_TransportedPionMap;
    typedef T_TransportedPionMap::const_iterator       CIT_TransportedPionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleAction_Transport( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_Transport();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    //@}

    //! @name Operations
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    //@}

    //! @name Event handler
    //@{
    virtual void NotifyComposanteChanged( const PHY_ComposantePion& composante );
    virtual void CheckConsistency();
    virtual void MagicLoadPion( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    virtual void MagicUnloadPion( MIL_Agent_ABC& pion );
    //@}

    //! @name Action
    //@{
    bool IsFinished    () const;
    bool IsTransporting() const;

    bool AddPion         ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    void Cancel          ();
    bool CanTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const;
    bool IsLoaded        ( const MIL_Agent_ABC& transported ) const;

    int  Load           ();
    void LoadSuspended  ();
    int  Unload         ( MT_Vector2D* position );
    void UnloadSuspended();
    //@}

    //! @name Notifications on transporter
    //@{
    virtual void ApplyContamination( const MIL_ToxicEffectManipulator& contamination ) ;
    virtual void ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination ) ;
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
        eUnloading,
        eErrorNoCarriers,
    };
    //@}

private:
    //! @name Helpers
    //@{
    bool HasChanged() const;
    void ComputeLoadingTime( double& rLoadingTime, double& rWeightToLoad ) const;
    double ComputeUnloadingTime() const;
    double DoLoad( const double rWeightToLoad );
    double DoUnload( const double rWeightToUnload, MT_Vector2D* position );
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RoleAction_Transport )
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&       owner_;
    bool                 bHasChanged_;
    E_State              nState_;
    bool                 bLoadUnloadHasBeenUpdated_;
    T_TransportedPionMap transportedPions_;
    double               rWeightTransported_;
    //@}
};

} // namespace transport

BOOST_CLASS_EXPORT_KEY( transport::PHY_RoleAction_Transport )
namespace transport
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RoleAction_Transport, MIL_AgentPion )
}

#endif // __PHY_RoleAction_Transport_h_
