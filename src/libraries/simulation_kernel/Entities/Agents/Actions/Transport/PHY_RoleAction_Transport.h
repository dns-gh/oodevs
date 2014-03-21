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
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "MT_Tools/Role_ABC.h"
#include <tools/Map.h>

namespace client
{
    class UnitAttributes;
}

class MIL_Agent_ABC;
class MIL_AgentPion;
class MT_Vector2D;
class PHY_ComposantePion;
class DEC_Decision_ABC;

namespace transport
{
// =============================================================================
// @class  PHY_RoleAction_Transport
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Transport : public tools::Role_ABC
                               , public transport::TransportNotificationHandler_ABC
                               , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Transport RoleInterface;
    //@}

private:
    //! @name Types
    //@{
    struct sTransportData
    {
        sTransportData();
        sTransportData( double rTotalWeight, bool bTransportOnlyLoadable, bool bMagic );

        bool   bTransportOnlyLoadable_;
        bool   bMagicLoad_;
        double rTotalWeight_;
        double rRemainingWeight_;
        double rTransportedWeight_;

        template< class Archive >
        void serialize( Archive & ar, unsigned int /*version*/ )
        {
            ar & bTransportOnlyLoadable_
               & bMagicLoad_
               & rTotalWeight_
               & rRemainingWeight_
               & rTransportedWeight_;
        }
    };

    typedef tools::Map< MIL_Agent_ABC*, sTransportData > T_TransportedPionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_Transport();
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
    std::vector< DEC_Decision_ABC* > GetTransportedUnits() const;
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
    double RemainingWeight( MIL_Agent_ABC& pion ) const;

    bool AddPion         ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    void Cancel          ();
    bool CanTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const;
    bool CanLoad         ( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const;
    double  GetNumberOfRoundTripToTransportPion( MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const;
    bool IsLoaded        ( const MIL_Agent_ABC& transported ) const;

    int  Load           ();
    void LoadSuspended  ();
    int  Unload         ( MT_Vector2D* position );
    void UnloadSuspended();
    //@}

    //! @name Notifications on transporter
    //@{
    virtual void ApplyContamination();
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

private:
    //! @name Member data
    //@{
    MIL_AgentPion*       owner_;
    E_State              nState_;
    bool                 bHasChanged_;
    bool                 bLoadUnloadHasBeenUpdated_;
    T_TransportedPionMap transportedPions_;
    double               rWeightTransported_;
    //@}
};

} // namespace transport

BOOST_CLASS_EXPORT_KEY( transport::PHY_RoleAction_Transport )

#endif // __PHY_RoleAction_Transport_h_
