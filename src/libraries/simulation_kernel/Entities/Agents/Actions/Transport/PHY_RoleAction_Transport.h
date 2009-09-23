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

#include "MT_Tools/Role_ABC.h"
#include "Entities/Agents/Roles/NBC/ToxicEffectHandler_ABC.h"

class NET_ASN_MsgUnitAttributes;
class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_ComposantePion;
class MIL_ToxicEffectManipulator;

namespace transport
{
	class TransportCapacityComputerFactory_ABC;

// =============================================================================
// @class  PHY_RoleAction_Transport
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Transport : public tools::Role_ABC
							   , public nbc::ToxicEffectHandler_ABC
                               , private boost::noncopyable
{

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
    explicit PHY_RoleAction_Transport( MIL_AgentPion& pion, const TransportCapacityComputerFactory_ABC& );
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
    //void NotifyComposanteContaminated( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void     ApplyContamination              ( const MIL_ToxicEffectManipulator& contamination ) ;
    virtual void     ApplyPoisonous                  ( const MIL_ToxicEffectManipulator& contamination ) ;
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
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_Transport* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_Transport* role, const unsigned int /*version*/ );

    //@}

private:
          MIL_AgentPion&        transporter_;
    const TransportCapacityComputerFactory_ABC& pTransportCapacityComputerFactory_;
          bool                  bHasChanged_;
          E_State               nState_;
          bool                  bLoadUnloadHasBeenUpdated_;
          T_TransportedPionMap  transportedPions_;
          MT_Float              rWeightTransported_; 
};

} // namespace transport

#endif // __PHY_RoleAction_Transport_h_
