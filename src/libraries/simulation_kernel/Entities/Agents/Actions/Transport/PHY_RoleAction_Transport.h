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

#include "MT_Tools/MT_Role_ABC.h"

class NET_ASN_MsgUnitAttributes;
class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_ComposantePion;
class MIL_NbcAgent;

// =============================================================================
// @class  PHY_RoleAction_Transport
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Transport : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Transport )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Transport RoleInterface;

    struct sTransportData
    {
        sTransportData();
        sTransportData( const sTransportData& rhs );
        sTransportData( MT_Float rTotalWeight, bool bTransportOnlyLoadable );

        template< typename Archive > void serialize( Archive&, const uint );

        const bool     bTransportOnlyLoadable_;
        const MT_Float rTotalWeight_;
              MT_Float rRemainingWeight_;
              MT_Float rTransportedWeight_;
      private:
        sTransportData& operator=( const sTransportData& rhs );
    };

    typedef std::map< MIL_Agent_ABC*, sTransportData > T_TransportedPionMap;
    typedef T_TransportedPionMap::iterator             IT_TransportedPionMap;
    typedef T_TransportedPionMap::const_iterator       CIT_TransportedPionMap;
    //@}

public:
             PHY_RoleAction_Transport( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_Transport();
    virtual ~PHY_RoleAction_Transport();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;

    void CheckConsistency();
    //@}

    //! @name Operations
    //@{
    int  GetInitialReturnCode() const;
    int  GetFinalReturnCode  () const;
    //@}

    //! @name Action
    //@{
    bool IsFinished    () const;
    bool IsTransporting() const;

    bool AddPion         ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    void Cancel          ();
    bool MagicLoadPion   ( MIL_Agent_ABC& pion, bool bTransportOnlyLoadable );
    bool MagicUnloadPion ( MIL_Agent_ABC& pion );
    bool CanTransportPion( const MIL_Agent_ABC& transported, bool bTransportOnlyLoadable ) const;
    bool IsLoaded        ( const MIL_Agent_ABC& transported ) const;

    int  Load           ();
    void LoadSuspended  ();
    int  Unload         ();
    void UnloadSuspended();
    //@}

    //! @name Notifications on transporter
    //@{
    void NotifyComposanteChanged     ( const PHY_ComposantePion& composante );
    void NotifyComposanteContaminated( const MIL_NbcAgent&       nbcAgent );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
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
    //! @name Tools
    //@{
    void     ComputeLoadingTime  ( MT_Float& rLoadingTime, MT_Float& rWeightToLoad ) const;
    MT_Float ComputeUnloadingTime() const;

    MT_Float DoLoad  ( const MT_Float rWeightToLoad   );
    MT_Float DoUnload( const MT_Float rWeightToUnload );
    //@}

private:
    const MIL_AgentPion*        pTransporter_;
          bool                  bHasChanged_;
          E_State               nState_;
          bool                  bLoadUnloadHasBeenUpdated_;
          T_TransportedPionMap  transportedPions_;
          MT_Float              rWeightTransported_; 
};

#include "PHY_RoleAction_Transport.inl"

#endif // __PHY_RoleAction_Transport_h_
