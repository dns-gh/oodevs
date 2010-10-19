// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef __PHY_RoleHLA_Composantes_h_
#define __PHY_RoleHLA_Composantes_h_

#include "PHY_RoleInterface_Composantes.h"
#include "protocol/ClientSenders.h"
#include "Decision/DEC_Decision_ABC.h"

class PHY_ComposanteHLA;
class HLA_InteractionManager_ABC;
class MIL_Entity_ABC;
class MIL_Object_ABC;

namespace hla
{
    class AttributeIdentifier;
    class Deserializer;
}

// =============================================================================
/** @class  PHY_RoleHLA_Composantes
    @brief  HLA role composantes
*/
// Created: AGE 2004-11-08
// =============================================================================
class PHY_RoleHLA_Composantes : public PHY_RoleInterface_Composantes
{
public:
    //! @name Constructors/Destructor
    //@{
            PHY_RoleHLA_Composantes( MIL_Entity_ABC& entity, HLA_InteractionManager_ABC& interactionManager );
    virtual ~PHY_RoleHLA_Composantes();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Volume*          GetSignificantVolume       ( const PHY_SensorTypeAgent& sensorType ) const;
    virtual       void                 GetVisibleVolumes          ( T_ComposanteVolumeSet& volumes         ) const;
    virtual       void                 BuildKnowledgeComposantes  ( T_KnowledgeComposanteVector& knowledge ) const;
    virtual       double             GetMajorComponentWeight    () const { return 0; };
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const;

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Destroy();
    //@}

    //! @name Fire
    //@{
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, unsigned int nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void Neutralize                 ();
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result, double );
    virtual void ApplyDirectFireOnMajorComposantes( const PHY_DotationCategory& /*dotationCategory*/, PHY_FireResults_ABC& /*fireResult*/ ) {}
    virtual void ApplyExplosion             ( const AttritionCapacity& objectType        , PHY_FireResults_ABC& result );
    virtual void ApplyContamination         ( const MIL_ToxicEffectManipulator& /*contamination*/ ) {};
    virtual void ApplyPoisonous             ( const MIL_ToxicEffectManipulator& /*contamination*/ ) {};
    virtual void ApplyUrbanObjectCrumbling  ( const MIL_Object_ABC& /*object*/ ) {};

    virtual double    GetDangerosity                     ( const DEC_Knowledge_AgentComposante& /*compTarget*/, float /*rDistBtwSourceAndTarget*/ ) const {return 0; }; 
    virtual double GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& /*target*/, double /*rWantedPH*/ ) const {return 0; };
    virtual double GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& /*target*/, double /*rWantedPH*/ ) const {return 0; };
    virtual double GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& /*target*/, double /*rWantedPH*/ ) const {return 0; };
    virtual double GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& /*target*/, double /*rWantedPH*/ ) const {return 0; };
    virtual double GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& /*target*/, double /*rWantedPH*/ ) const {return 0; };
    virtual double GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& /*dotationCategory*/, bool /*bCheckDotationsAvailability*/ ) const {return 0; };
    virtual double GetMinRangeToIndirectFire       ( const PHY_DotationCategory& /*dotationCategory*/, bool /*bCheckDotationsAvailability*/ ) const {return 0; };
    virtual double GetMaxRangeToFire                 ( const MIL_Agent_ABC& /*pion*/, double /*rWantedPH*/ ) const { return 0; };
    virtual void ApplyInjury                ( MIL_Injury_ABC& injury );
    //@}

    //! @name Accessors
    //@{
    virtual bool     IsNeutralized           () const;
    virtual double GetOperationalState     () const;
    virtual double GetMajorOperationalState() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

    virtual void Update( bool /*bIsDead*/ ) {}
    virtual void Clean () {}
    //@}

    //! @name Humans management
    //@{
    virtual void WoundHumans  ( const PHY_HumanRank& /*rank*/, unsigned int /*nNbr*/ ) {};
    virtual void HealHumans   ( const PHY_HumanRank& /*rank*/, unsigned int /*nNbr*/ ) {};
    virtual void HealAllHumans() {};
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& /*composanteType*/, unsigned int /*nNbrAvailable*/ ) {}
    virtual void RepairAllComposantes         () {}
    virtual void DestroyRandomComposante      () {}
    virtual void DestroyAllComposantes        () {}
    //@}

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& /*borrower*/, PHY_ComposantePion& /*composante*/ ) {}
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& /*borrower*/, PHY_ComposantePion& /*composante*/ ) {}

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& /*lender*/, PHY_ComposantePion& /*composante*/ ) {}
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& /*lender*/, PHY_ComposantePion& /*composante*/ ) {}
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait être externalisée
    virtual void GetConvoyTransportersUse( T_ComposanteUseMap& composanteUse ) const;

    //! @name Logistic - maintenance
    //@{
    virtual void                            PreprocessRandomBreakdowns           ( unsigned int /*nEndDayTimeStep*/ ) const {}

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& /*composante*/ ) { return NULL; }
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& /*composanteState*/ ) {}

    //$$$$ a deplacer (functor)
    virtual PHY_ComposantePion*             GetAvailableHauler                   ( const PHY_ComposanteTypePion& /*composanteType*/ ) const { return NULL; }
    //@}

    //! @name Prisoners
    //@{
    virtual void NotifyCaptured() {}
    virtual void NotifyReleased() {}
    //@}

    //! @name Load / unload / transport
    //@{
    virtual void DamageTransported( double /*rWeight*/, const PHY_ComposanteState& /*state*/, bool /*bTransportOnlyLoadable*/ ) const {}
    //@}

    //! @name Notifications (internal)
    //@{
    virtual void NotifyComposanteAdded   ( PHY_ComposantePion& /*composante*/ ) {}
    virtual void NotifyComposanteRemoved ( PHY_ComposantePion& /*composante*/ ) {}
    virtual void NotifyComposanteChanged ( PHY_ComposantePion& /*composante*/, const PHY_ComposanteState& /*oldState*/ ) {}
    virtual void NotifyComposanteRepaired() {}
    //@}

    //! @name Accessors
    //@{
    virtual       bool           HasChanged              () const { return false; }
    virtual       bool           IsUsable                () const { return false; }
    virtual const MIL_Agent_ABC& GetPion                 () const { return *pPion_; }
    //@}


    //! @name Network
    //@{
//    virtual void SendChangedState( client::UnitAttributes& asn ) const {};
//    virtual void SendFullState   ( client::UnitAttributes& asn ) const {};

    virtual void SendLogisticChangedState() const {};
    virtual void SendLogisticFullState   () const {};
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& /*functor*/ ) const {}
    //@}

    //! @name Composante reserved
    //@{
    virtual void WoundLoadedHumans( const PHY_ComposantePion& /*composanteChanged*/, const PHY_ComposanteState& /*newState*/, PHY_FireDamages_Agent& /*fireDamages*/ ) {}
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_ComposanteHLA* > T_Composantes;
    typedef T_Composantes::const_iterator   CIT_Composantes;
    //@}

    //! @name Helpers
    //@{
    void UpdateComposante( const std::string& strComposanteName, unsigned int nComposantes );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    T_Composantes               composantes_;
    HLA_InteractionManager_ABC& interactionManager_;
    bool                        bNeutralized_;
    MIL_Agent_ABC*              pPion_;
    //@}
};

#endif // __PHY_RoleHLA_Composantes_h_
