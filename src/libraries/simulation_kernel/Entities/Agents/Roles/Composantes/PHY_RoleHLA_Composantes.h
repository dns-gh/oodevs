// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.h $
// $Author: Age $
// $Modtime: 13/12/04 10:34 $
// $Revision: 8 $
// $Workfile: PHY_RoleHLA_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Composantes_h_
#define __PHY_RoleHLA_Composantes_h_

#include "PHY_RoleInterface_Composantes.h"
#include "hla/Deserializer.h"

class PHY_ComposanteHLA;
class HLA_InteractionManager_ABC;
class MIL_Entity_ABC;

namespace hla {
    class AttributeIdentifier;    
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
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const;

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Destroy();
    //@}

    //! @name Fire
    //@{
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, uint nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void Neutralize                 ();
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyExplosion             ( const AttritionCapacity& objectType        , PHY_FireResults_ABC& result );
    virtual void ApplyContamination         ( const MIL_ToxicEffectManipulator& contamination ) {};
    virtual void ApplyPoisonous             ( const MIL_ToxicEffectManipulator& contamination ) {};
    virtual MT_Float GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const {return 0; };
    virtual MT_Float GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const {return 0; };
    virtual MT_Float GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const {return 0; };
    virtual MT_Float GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const {return 0; };
    virtual MT_Float GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const {return 0; };
    virtual MT_Float GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const {return 0; };
    virtual MT_Float GetMinRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const {return 0; };
    virtual void ApplyInjury                ( MIL_Injury_ABC& injury );
    //@}

    //! @name Accessors
    //@{
    virtual bool     IsNeutralized           () const;
    virtual MT_Float GetOperationalState     () const;
    virtual MT_Float GetMajorOperationalState() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

    virtual void Update( bool bIsDead ) {};
    virtual void Clean () {};
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, uint nNbrAvailable ) {};
    virtual void RepairAllComposantes         () {};
    virtual void DestroyRandomComposante      () {};
    virtual void DestroyAllComposantes        () {};
    //@}

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) {};
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) {};

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) {}; 
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) {};
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait être externalisée

    //! @name Logistic - maintenance
    //@{
    virtual void                            PreprocessRandomBreakdowns           ( uint nEndDayTimeStep ) const {};

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante ) { return NULL; };
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState ) {};

    //$$$$ a deplacer (functor)
    virtual PHY_ComposantePion*             GetAvailableHauler                   ( const PHY_ComposanteTypePion& composanteType ) const { return NULL; };
    //@}

    //! @name Prisoners
    //@{
    virtual void NotifyCaptured() {};
    virtual void NotifyReleased() {};
    //@}

    //! @name Load / unload / transport
    //@{
    virtual void DamageTransported( MT_Float rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const {};
    //@}

    //! @name Notifications (internal)
    //@{
    virtual void NotifyComposanteAdded   ( PHY_ComposantePion& composante ) {};
    virtual void NotifyComposanteRemoved ( PHY_ComposantePion& composante ) {};
    virtual void NotifyComposanteChanged ( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState ) {};
    virtual void NotifyComposanteRepaired() {};
    //@}

    //! @name Accessors
    //@{          
    virtual       bool           HasChanged              () const { return false; };
    virtual       bool           IsUsable                () const { return false; };                    
    virtual const MIL_AgentPion& GetPion                 () const { return *pPion_; };    
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const {};
    //@}

    //! @name Composante reserved
    //@{
    virtual void WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages ) {};
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_ComposanteHLA* > T_Composantes;
    typedef T_Composantes::const_iterator   CIT_Composantes;
    //@}

    //! @name Helpers
    //@{
    void UpdateComposante( const std::string& strComposanteName, uint nComposantes );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    T_Composantes               composantes_;
    HLA_InteractionManager_ABC& interactionManager_;
    bool                        bNeutralized_;
    MIL_AgentPion*         pPion_;
    //@}
};

#endif // __PHY_RoleHLA_Composantes_h_
