// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:16 $
// $Revision: 21 $
// $Workfile: PHY_RolePion_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Composantes_h_
#define __PHY_RolePion_Composantes_h_

#include "PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;
class MIL_ToxicEffectManipulator;
class MIL_AutomateLOG;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_Human;
class PHY_UnitType;
class PHY_ComposanteState;
class PHY_DirectFireData;
class PHY_IndirectFireData;
class PHY_SmokeData;
class PHY_FireDamages_Agent;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class HLA_UpdateFunctor;

namespace xml
{
    class xostream;
    class xistream;
}

// =============================================================================
// @class  PHY_RolePion_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Composantes : public PHY_RoleInterface_Composantes
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
    //@}

public:
    explicit PHY_RolePion_Composantes( MIL_AgentPion& pion );
             PHY_RolePion_Composantes();
    virtual ~PHY_RolePion_Composantes();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Operations 
    //@{
    template< typename T > void                Apply              ( T& t ) const;
    template< typename T > PHY_ComposantePion* GetComposante      ( T& t ) const;
    template< typename T > void                GetComposantesUse  ( T_ComposanteUseMap& composanteUse, T& t ) const;
    template< typename T > bool                HasUsableComposante( T& t ) const;

    template< typename T > void                ApplyOnWeapons     ( T& t ) const;


    void Update( bool bIsDead );
    void Clean ();
    //@}

    //! @name Humans management
    //@{
    void WoundHumans  ( const PHY_HumanRank& rank, uint nNbr );
    void HealHumans   ( const PHY_HumanRank& rank, uint nNbr );    
    void HealAllHumans();
    //@}

    //! @name Composantes management
    //@{
    void ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, uint nNbrAvailable );
    void RepairAllComposantes         ();
    void DestroyRandomComposante      ();
    void DestroyAllComposantes        ();
    //@}

    //! @name Pret de composantes
    //@{
    template < typename T > uint LendComposantes             ( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );
    template < typename T > uint GetLentComposantesTravelTime( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );
    template < typename T > uint RetrieveLentComposantes     ( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate );

    // Actions on the composante owner
    void LendComposante        ( PHY_RolePion_Composantes& borrower, PHY_ComposantePion& composante );
    void RetrieveLentComposante( PHY_RolePion_Composantes& borrower, PHY_ComposantePion& composante );

    // Notification for the beneficary
    void NotifyLentComposanteReceived( PHY_RolePion_Composantes& lender, PHY_ComposantePion& composante ); 
    void NotifyLentComposanteReturned( PHY_RolePion_Composantes& lender, PHY_ComposantePion& composante );
    //@}

    //$$$ Toute la partie logistique GetXXXUse() devrait être externalisée

    //! @name Logistic - maintenance
    //@{
    void                            PreprocessRandomBreakdowns           ( uint nEndDayTimeStep ) const;

    PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante );
    void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState );

    //$$$$ a deplacer (functor)
    PHY_ComposantePion*             GetAvailableHauler                   ( const PHY_ComposanteTypePion& composanteType ) const;
    //@}

    //! @name Logistic - Medical
    //@{
    bool                   HasWoundedHumansToEvacuate      () const;
    void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;
    PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 );
    PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human );
    void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState );
    //@}

    //! @name Logistic - Supply
    //@{

    //$$$$ a deplacer (functor)
    PHY_ComposantePion* GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const;
    void                GetConvoyTransportersUse     ( T_ComposanteUseMap& composanteUse ) const;
    //@}

    //! @name Prisoners
    //@{
    void NotifyCaptured();
    void NotifyReleased();
    //@}

    //! @name Fire / Dangerosity
    //@{
            bool     IsNeutralized                   () const;
    virtual void     GetComposantesAbleToBeFired     ( T_ComposanteVector& targets, uint nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void     GetComposantesAbleToBeFired     ( T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void     Neutralize                      ();
    virtual void     ApplyPopulationFire             ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void     ApplyDirectFire                 ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyIndirectFire               ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyExplosion                  ( const AttritionCapacity& capacity, PHY_FireResults_ABC& result );
            void     ApplyContamination              ( const MIL_ToxicEffectManipulator& contamination );
            void     ApplyPoisonous                  ( const MIL_ToxicEffectManipulator& contamination );
            MT_Float GetOnlyLoadableMaxRangeToFireOn ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMinRangeToFireOn             ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
            MT_Float GetMinRangeToIndirectFire       ( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const;
            MT_Float GetDangerosity                  ( const DEC_Knowledge_Agent& target ) const;
    //@}

    //! @name Perception / Knowledge
    //@{
    virtual const PHY_Volume*          GetSignificantVolume     ( const PHY_SensorTypeAgent& sensorType  ) const;
    virtual       void                 GetVisibleVolumes        ( T_ComposanteVolumeSet& volumes         ) const;
    virtual       void                 BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const;
    virtual const PHY_ComposantePion*  GetMajorComposante       () const;
    //@}

    //! @name Load / unload / transport
    //@{
    void DamageTransported( MT_Float rWeight, const PHY_ComposanteState& state, bool bTransportOnlyLoadable ) const;
    //@}

    //! @name Notifications (internal)
    //@{
    void NotifyComposanteAdded   ( PHY_ComposantePion& composante );
    void NotifyComposanteRemoved ( PHY_ComposantePion& composante );
    void NotifyComposanteChanged ( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState );
    void NotifyComposanteRepaired();

    void NotifyHumanAdded  ( PHY_Human& human );
    void NotifyHumanRemoved( PHY_Human& human );
    void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
    //@}

    //! @name Accessors
    //@{          
    virtual       MT_Float       GetOperationalState     () const;
    virtual       MT_Float       GetMajorOperationalState() const;
                  bool           HasChanged              () const;
                  bool           IsUsable                () const;                    
            const MIL_AgentPion& GetPion                 () const;    
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const;

    void SendLogisticChangedState() const;
    void SendLogisticFullState   () const;
    //@}

    //! @name HLA
    //@{
    void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Composante reserved
    //@{
    void WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages );
    //@}

private:
    //! @name Types
    //@{
    struct T_ComposanteTypeProperties
    {
        T_ComposanteTypeProperties();

        bool HasUsableComposantes() const;
        template< typename Archive > void serialize( Archive&, const uint );

        std::vector< uint > nbrsPerState_;
        bool                bHasChanged_;
    };
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< PHY_ComposantePion* >       T_ComposantePionVector;
    typedef T_ComposantePionVector::iterator         IT_ComposantePionVector;
    typedef T_ComposantePionVector::const_iterator   CIT_ComposantePionVector;
    typedef T_ComposantePionVector::reverse_iterator RIT_ComposantePionVector;
    
    typedef std::set< PHY_MaintenanceComposanteState* >     T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator       IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;
    
    typedef std::map< const PHY_RolePion_Composantes*, T_ComposantePionVector > T_LoanMap;
    typedef T_LoanMap::iterator                                                 IT_LoanMap;
    typedef T_LoanMap::const_iterator                                           CIT_LoanMap;

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                         IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                   CIT_ComposanteTypeMap;    
    //@}

private:
    //! @name Init
    //@{
    void DistributeCommanders      ();
    void DistributeHumanWounds     ( const PHY_HumanRank& rank, const PHY_HumanWound& wound, uint nNbr, CIT_ComposantePionVector& itCurrentComp );
    void ReadHumansOverloading     ( xml::xistream& xis );
    void ReadComposantesOverloading( xml::xistream& xis );
    //@}

    //! @name Tools
    //@{
    void UpdateOperationalStates();
    void UpdateMajorComposante  ();

    void UpdateDataWhenComposanteRemoved( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );
    void UpdateDataWhenComposanteAdded  ( const PHY_ComposanteState& state, T_ComposanteTypeProperties& properties );

    void SendLoans( NET_ASN_MsgUnitAttributes& asn ) const;
    //@}
    //! @name Helpers
    //@{
    void ReadEquipements( xml::xistream& xis );
    void ReadEquipement ( xml::xistream& xis );
    void ReadPersonnels ( xml::xistream& xis );
    void ReadPersonnel  ( xml::xistream& xis );

    //@}

private:
    MIL_AgentPion*         pPion_;
    T_ComposantePionVector composantes_;
    T_ComposanteTypeMap    composanteTypes_;
    uint                   nNbrComposanteChanged_;
    MT_Float               rMajorOperationalState_;
    MT_Float               rOperationalState_;
    bool                   bOperationalStateChanged_;    
    PHY_ComposantePion*    pMajorComposante_;
    uint                   nNeutralizationEndTimeStep_;
    uint                   nNbrUsableComposantes_;

    T_LoanMap              lentComposantes_;
    T_LoanMap              borrowedComposantes_;

    bool                   bLoansChanged_;

    // Maintenance
    T_MaintenanceComposanteStateSet maintenanceComposanteStates_;
    uint                            nTickRcMaintenanceQuerySent_;

public:
    static MT_Float rOpStateWeightNonMajorComposante_;
    static MT_Float rOpStateWeightMajorComposante_;
    static MT_Float rMaxDangerosityDegradationByNeutralizedState_;
    static MT_Float rMaxDangerosityDegradationByOpState_;
};

#include "PHY_RolePion_Composantes.inl"

#endif // __PHY_RolePion_Composantes_h_
