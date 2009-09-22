// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h $
// $Author: Jvt $
// $Modtime: 31/03/05 18:21 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Composantes_h_
#define __PHY_RoleInterface_Composantes_h_

#include "MT_Tools/Role_ABC.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

class PHY_Composante_ABC;
class PHY_SensorTypeAgent;
class PHY_Volume;
class PHY_FireResults_ABC;
class PHY_DotationCategory;
class AttritionCapacity;
class MIL_PopulationType;
class MIL_PopulationAttitude;
class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;
class MIL_ToxicEffectManipulator;
class MIL_AutomateLOG;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_Human;
class PHY_UnitType;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class HLA_UpdateFunctor;
class PHY_ComposanteTypePion;

namespace firing
{
    class PHY_SmokeData;
}

// =============================================================================
// @class  PHY_RoleInterface_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Composantes : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    struct T_ComposanteUse
    {
        uint nNbrAvailable_;
        uint nNbrUsed_;
        uint nNbrTotal_; // nNbrTotal_ >= nNbrAvailable_ >= nNbrUsed_
        uint nNbrLent_;
    };

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;

    typedef PHY_RoleInterface_Composantes RoleInterface;

    typedef std::set< PHY_MaintenanceComposanteState* >     T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator       IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;

    typedef std::map< const PHY_RoleInterface_Composantes*, PHY_ComposantePion::T_ComposantePionVector > T_LoanMap;
    typedef T_LoanMap::iterator                                                 IT_LoanMap;
    typedef T_LoanMap::const_iterator                                           CIT_LoanMap;

    /*typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                         IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                   CIT_ComposanteTypeMap;   

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;
   */ //@}

public:
             PHY_RoleInterface_Composantes();
    virtual ~PHY_RoleInterface_Composantes();


    virtual void Update( bool bIsDead ) = 0;
    virtual void Clean () = 0;
    //@}

    //! @name Humans management
    //@{
    virtual void WoundHumans  ( const PHY_HumanRank& rank, uint nNbr ) = 0;
    virtual void HealHumans   ( const PHY_HumanRank& rank, uint nNbr ) = 0;    
    virtual void HealAllHumans() = 0 ;
    //@}

    //! @name Composantes management
    //@{
    virtual void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, uint nNbrAvailable ) = 0;
    virtual void RepairAllComposantes         () = 0;
    virtual void DestroyRandomComposante      () = 0;
    virtual void DestroyAllComposantes        () = 0;
    //@}

    // Actions on the composante owner
    virtual void LendComposante        ( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) = 0;
    virtual void RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante ) = 0;

    // Notification for the beneficary
    virtual void NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) = 0; 
    virtual void NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante ) = 0;
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait �tre externalis�e

    //! @name Logistic - maintenance
    //@{
    virtual void                            PreprocessRandomBreakdowns           ( uint nEndDayTimeStep ) const = 0;

    virtual PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante ) = 0;
    virtual void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState ) = 0;

    //$$$$ a deplacer (functor)
    virtual PHY_ComposantePion*             GetAvailableHauler                   ( const PHY_ComposanteTypePion& composanteType ) const = 0;
    //@}

    //! @name Logistic - Medical
    //@{
    virtual bool                   HasWoundedHumansToEvacuate      () const = 0;
    virtual void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const = 0;
    virtual PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) = 0;
    virtual PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human ) = 0;
    virtual void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState ) = 0;
    //@}

    //! @name Logistic - Supply
    //@{

    //$$$$ a deplacer (functor)
    virtual PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual void                GetConvoyTransportersUse     ( T_ComposanteUseMap& composanteUse ) const = 0;
    //@}

    //! @name Prisoners
    //@{
    virtual void NotifyCaptured() = 0;
    virtual void NotifyReleased() = 0;
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Volume*          GetSignificantVolume       ( const PHY_SensorTypeAgent& sensorType ) const = 0;
    virtual       void                 GetVisibleVolumes          ( T_ComposanteVolumeSet& volumes         ) const = 0;
    virtual       void                 BuildKnowledgeComposantes  ( T_KnowledgeComposanteVector& knowledge ) const = 0;
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const = 0;
    virtual       MT_Float             GetOperationalState        () const = 0;
    virtual       MT_Float             GetMajorOperationalState   () const = 0;
    //@}

    //! @name Fire
    //@{
    virtual bool IsNeutralized              () const = 0;
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, uint nNbrFirers, bool bFireOnlyOnMajorComposantes = false ) const = 0;//TODO MGD Remove and use ComposantesAbleToBeFired
    virtual void GetComposantesAbleToBeFired( PHY_Composante_ABC::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const = 0;
    virtual void Neutralize                 () = 0;
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyExplosion             ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result ) = 0;
    virtual MT_Float GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const = 0;
    virtual MT_Float GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const = 0;
    virtual MT_Float GetMinRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const = 0;
    virtual MT_Float GetDangerosity                  ( const DEC_Knowledge_Agent& target ) const = 0;
    //@}


    //! @name Load / unload / transport
    //@{
    virtual void DamageTransported( MT_Float rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const = 0;
    //@}

    //! @name Notifications (internal)
    //@{
    virtual void NotifyComposanteAdded   ( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteRemoved ( PHY_ComposantePion& composante ) = 0;
    virtual void NotifyComposanteChanged ( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState ) = 0;
    virtual void NotifyComposanteRepaired() = 0;

    virtual void NotifyHumanAdded  ( PHY_Human& human ) = 0;
    virtual void NotifyHumanRemoved( PHY_Human& human ) = 0;
    virtual void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState ) = 0;
    //@}

    //! @name Accessors
    //@{          
    virtual       bool           HasChanged              () const = 0;
    virtual       bool           IsUsable                () const = 0;                    
    virtual const MIL_AgentPion& GetPion                 () const = 0;    
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const = 0;

    virtual void SendLogisticChangedState() const = 0;
    virtual void SendLogisticFullState   () const = 0;
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const = 0;
    //@}

    //! @name Composante reserved
    //@{
    virtual void WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages ) = 0;
    //@}

public:
    static MT_Float rOpStateWeightNonMajorComposante_;
    static MT_Float rOpStateWeightMajorComposante_;
    static MT_Float rMaxDangerosityDegradationByNeutralizedState_;
    static MT_Float rMaxDangerosityDegradationByOpState_;

private:
    //! @name Serialization
	//@{
	friend class boost::serialization::access;
	template< typename Archive > void serialize( Archive& ar, const uint )
	{
		ar & boost::serialization::base_object< tools::Role_ABC >( *this );
	}
	//@}
};

#endif // __PHY_RoleInterface_Composantes_h_
