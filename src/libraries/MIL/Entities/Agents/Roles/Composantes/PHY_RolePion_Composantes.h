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

#include "MIL.h"

#include "PHY_RoleInterface_Composantes.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

class NET_ASN_MsgUnitDotations;
class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;
class MIL_NbcAgentType;
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

// =============================================================================
// @class  PHY_RolePion_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Composantes : public PHY_RoleInterface_Composantes
{
    MT_COPYNOTALLOWED( PHY_RolePion_Composantes )

public:
    //! @name Types
    //@{
    struct T_ComposanteUse
    {
        uint nNbrAvailable_;
        uint nNbrUsed_;
    };

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteUse > T_ComposanteUseMap;
    typedef T_ComposanteUseMap::const_iterator                         CIT_ComposanteUseMap;
    //@}

public:
             PHY_RolePion_Composantes( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Composantes();
    virtual ~PHY_RolePion_Composantes();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    void ReadOverloading( MIL_InputArchive& archive );
    //@}

    //! @name Operations 
    //@{
    template< typename T > void Apply ( T& t ) const;

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
    uint LendCollectionComposantes       ( PHY_RolePion_Composantes& newRole, uint nNbr );
    uint GetLendCollectionComposantesTime( PHY_RolePion_Composantes& newRole, uint nNbr );
    uint UndoLendCollectionComposantes   ( PHY_RolePion_Composantes& role,    uint nNbr );

    void LendComposante    ( PHY_RolePion_Composantes& newRole, PHY_ComposantePion& composante );
    void UndoLendComposante( PHY_RolePion_Composantes& newRole, PHY_ComposantePion& composante );
    //@}

    //! @name Logistic - maintenance
    //@{
    void                            PreprocessRandomBreakdowns           ( uint nEndDayTimeStep ) const;

    PHY_MaintenanceComposanteState* NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante );
    void                            NotifyComposanteBackFromMaintenance  ( PHY_MaintenanceComposanteState& composanteState );

    bool                            HasUsableHauler                      ( const PHY_ComposanteTypePion& composanteType ) const;
    PHY_ComposantePion*             GetAvailableHauler                   ( const PHY_ComposanteTypePion& composanteType ) const;
    void                            GetHaulersUse                        ( T_ComposanteUseMap& composanteUse ) const; 
    bool                            HasUsableRepairer                    ( const PHY_Breakdown& breakdown ) const;
    PHY_ComposantePion*             GetAvailableRepairer                 ( const PHY_Breakdown& breakdown ) const;    
    void                            GetRepairersUse                      ( T_ComposanteUseMap& composanteUse ) const; 
    //@}

    //! @name Logistic - Medical
    //@{
    bool                   HasWoundedHumansToEvacuate      () const;
    void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;
    PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 );
    PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human );
    void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState );

    bool                   HasUsableEvacuationAmbulance    () const;
    PHY_ComposantePion*    GetAvailableEvacuationAmbulance () const;
    void                   GetEvacuationAmbulancesUse      ( T_ComposanteUseMap& composanteUse ) const;
    bool                   HasUsableCollectionAmbulance    () const;
    PHY_ComposantePion*    GetAvailableCollectionAmbulance () const;
    void                   GetCollectionAmbulancesUse      ( T_ComposanteUseMap& composanteUse ) const;

    void                   GetDoctorsUse                   ( T_ComposanteUseMap& composanteUse ) const;
    PHY_ComposantePion*    GetAvailableDoctorForDiagnosing () const; 
    bool                   HasUsableDoctorForSorting       () const;
    PHY_ComposantePion*    GetAvailableDoctorForSorting    () const;
    void                   GetDoctorsUseForSorting         ( T_ComposanteUseMap& composanteUse ) const;
    bool                   HasUsableDoctorForHealing       () const;
    bool                   HasUsableDoctorForHealing       ( const PHY_Human& human ) const;    
    PHY_ComposantePion*    GetAvailableDoctorForHealing    ( const PHY_Human& human ) const;
    //@}

    //! @name Logistic - Supply
    //@{
    PHY_ComposantePion* GetAvailableConvoyTransporter() const;
    void                GetConvoyTransporters        ( T_ComposanteUseMap& composanteUse ) const;
    PHY_ComposantePion* GetAvailableConvoyCommander  () const;
    void                GetConvoyCommanders          ( T_ComposanteUseMap& composanteUse ) const;    
    //@}

    //! @name Prisoners
    //@{
    void NotifyTakenPrisoner();
    //@}

    //! @name Fire / Dangerosity
    //@{
            void     FillDirectFireData             ( PHY_DirectFireData& data ) const;
            void     FillIndirectFireData           ( PHY_IndirectFireData& data ) const;
            void     FillIndirectFireData           ( PHY_SmokeData& data ) const;
            bool     IsNeutralized                  () const;
    virtual void     GetComposantesAbleToBeFired    ( T_ComposanteVector& targets, uint nNbrFirer ) const;
    virtual void     GetComposantesAbleToBeFired    ( T_ComposanteVector& targets ) const;
    virtual void     Neutralize                     ();
    virtual void     ApplyPopulationFire            ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void     ApplyDirectFire                ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyIndirectFire              ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void     ApplyExplosion                 ( const MIL_RealObjectType& objectType, PHY_FireResults_ABC& result );
            void     ApplyContamination             ( const MIL_NbcAgentType& nbcAgentType );
            MT_Float GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToFireOn            ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMinRangeToFireOn            ( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const;
            MT_Float GetMaxRangeToIndirectFire      ( const PHY_IndirectFireDotationClass& ) const;
            MT_Float GetMinRangeToIndirectFire      ( const PHY_IndirectFireDotationClass& ) const;
            MT_Float GetDangerosity                 ( const DEC_Knowledge_Agent& target ) const;
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
          bool           HasChanged() const;
          bool           IsUsable  () const;  
          MT_Float       GetEtatOps() const;
    const MIL_AgentPion& GetPion   () const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitDotations& asn ) const;
    void SendFullState   ( NET_ASN_MsgUnitDotations& asn ) const;

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

public:
    typedef std::vector< PHY_ComposantePion* >       T_ComposantePionVector;
    typedef T_ComposantePionVector::iterator         IT_ComposantePionVector;
    typedef T_ComposantePionVector::const_iterator   CIT_ComposantePionVector;
    typedef T_ComposantePionVector::reverse_iterator RIT_ComposantePionVector;
    
    typedef std::set< PHY_MaintenanceComposanteState* >     T_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::iterator       IT_MaintenanceComposanteStateSet;
    typedef T_MaintenanceComposanteStateSet::const_iterator CIT_MaintenanceComposanteStateSet;
    
    typedef std::map< const PHY_RolePion_Composantes*, T_ComposantePionVector > T_LentComposanteMap;
    typedef T_LentComposanteMap::iterator                                       IT_LentComposanteMap;
    typedef T_LentComposanteMap::const_iterator                                 CIT_LentComposanteMap;

    typedef std::map< const PHY_ComposanteTypePion*, T_ComposanteTypeProperties > T_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::iterator                                         IT_ComposanteTypeMap;
    typedef T_ComposanteTypeMap::const_iterator                                   CIT_ComposanteTypeMap;    
    //@}

private:
    //! @name Init
    //@{
    void DistributeCommanders      ();
    void DistributeHumanWounds     ( const PHY_HumanRank& rank, const PHY_HumanWound& wound, uint nNbr, CIT_ComposantePionVector& itCurrentComp );
    void ReadHumansOverloading     ( MIL_InputArchive& archive );
    void ReadComposantesOverloading( MIL_InputArchive& archive );
    //@}

    //! @name Tools
    //@{
    void UpdateEtatOps        ();
    void UpdateMajorComposante();

    void UpdateDataWhenComposanteRemoved( const PHY_ComposanteState& state, bool bIsMajor, T_ComposanteTypeProperties& properties );
    void UpdateDataWhenComposanteAdded  ( const PHY_ComposanteState& state, bool bIsMajor, T_ComposanteTypeProperties& properties );

    void SendChangedLends     ( NET_ASN_MsgUnitDotations& asn ) const;
    void SendFullLends        ( NET_ASN_MsgUnitDotations& asn ) const;
    //@}

private:
    MIL_AgentPion*         pPion_;
    T_ComposantePionVector composantes_;
    T_LentComposanteMap    lentComposantes_;
    bool                   bLendsChanged_;
    T_ComposanteTypeMap    composanteTypes_;
    uint                   nNbrComposanteChanged_;
    MT_Float               rEtatOps_;
    bool                   bEtatOpsChanged_;    
    PHY_ComposantePion*    pMajorComposante_;
    uint                   nNeutralizationEndTimeStep_;

    uint                   nNbrUsableComposantes_;

    uint                   nNbrUndamagedMajorComposantes_;
    uint                   nNbrUndamagedNonMajorComposantes_;
    uint                   nNbrMajorComposantes_;
    uint                   nNbrNonMajorComposantes_;

    // Maintenance
    T_MaintenanceComposanteStateSet maintenanceComposanteStates_;
    uint                            nTickRcMaintenanceQuerySent_;

public:
    static MT_Float rEtatOpsWeightNonMajorComposante_;
    static MT_Float rEtatOpsWeightMajorComposante_;
    static MT_Float rEtatOpsWeightHumans_;
    static MT_Float rMaxDangerosityDegradationByNeutralizedState_;
    static MT_Float rMaxDangerosityDegradationByEtatOps_;
};

#include "PHY_RolePion_Composantes.inl"

#endif // __PHY_RolePion_Composantes_h_
