// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AutomateLOG_h_
#define __MIL_AutomateLOG_h_

#include "MIL.h"
#include "Entities/Actions/PHY_Actor.h"
#include "Entities/Specialisations/LOG/LogisticHierarchyOwner_ABC.h"
#include "Entities/Agents/Roles/Logistic/SupplySupplier_ABC.h"
#include "Entities/Agents/Roles/Logistic/FuneralHandler_ABC.h"
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
    class xistream;
}

namespace sword
{
    class ParentEntity;
    class MissionParameters;
    class UnitMagicAction;
    class PushFlowParameters;
}

namespace logistic
{
    class LogisticHierarchy;
    class LogisticHierarchy_ABC;
    class SupplyRequestContainer;
    class SupplyConsign_ABC;
    class SupplyRequest_ABC;
}

class AutomateFactory_ABC;
class Human_ABC;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_AutomateTypeLOG;
class MIL_Formation;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class PHY_MedicalCollectionAmbulance;
class PHY_RoleInterface_Medical;
class PHY_RoleInterface_Maintenance;
class PHY_Dotation;
class PHY_RoleInterface_Supply;
class PHY_DotationCategory;
class PHY_DotationType;
class PHY_ComposantePion;
class PHY_LogisticLevel;

// =============================================================================
// @class  MIL_AutomateLOG
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateLOG : public PHY_Actor
                      , public logistic::LogisticHierarchyOwner_ABC
                      , public logistic::SupplySupplier_ABC
                      , public logistic::FuneralHandler_ABC
{
public:
             MIL_AutomateLOG( MIL_Formation& formation, const PHY_LogisticLevel& level );
             MIL_AutomateLOG( MIL_Automate&  automat, const PHY_LogisticLevel& level );
    virtual ~MIL_AutomateLOG();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Initialize
    //@{
    virtual void ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    void UpdateLogistic();
    void Clean         ();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetLogisticId() const;
    virtual unsigned int GetSupplierId() const;

    logistic::LogisticHierarchy_ABC&  GetLogisticHierarchy() const;
    MIL_Army_ABC&                     GetArmy     () const;
    virtual const MIL_AgentPion*      GetPC      () const;
    const PHY_LogisticLevel&          GetLogisticLevel() const;
    //@}

    //! @name Maintenance
    //@{
    boost::shared_ptr< PHY_MaintenanceComposanteState > MaintenanceHandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante );
    bool                            MaintenanceHandleComposanteForTransport   ( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    bool                            MaintenanceHandleComposanteForDiagnosis   ( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    bool                            MaintenanceHandleComposanteForRepair      ( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
    bool                            IsMaintenanceManual() const;
    bool                            IsSupplyManual() const;
    void                            OnReceiveLogMaintenanceSetManual          ( const sword::MissionParameters& parameters );
    void                            OnReceiveLogSupplySetManual               ( const sword::MissionParameters& parameters );

    //$$$ A FACTORISER AVEC LES FONCTION CI DESSUS
    PHY_RoleInterface_Maintenance*  MaintenanceFindAlternativeTransportHandler( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state, const PHY_ComposanteTypePion* type = 0 );
    PHY_RoleInterface_Maintenance*  MaintenanceFindAlternativeDiagnosisHandler( const PHY_ComposanteTypePion* type = 0 );
    PHY_RoleInterface_Maintenance*  MaintenanceFindAlternativeRepairHandler   ( const boost::shared_ptr< PHY_MaintenanceComposanteState >& state, const PHY_ComposanteTypePion* type = 0 );
    //@}

    //! @name Medical
    //@{
    boost::shared_ptr< PHY_MedicalHumanState > MedicalHandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human ); // Imex
    boost::shared_ptr< PHY_MedicalHumanState > MedicalHandleHumanForEvacuation        ( MIL_AgentPion& pion, Human_ABC& human ); // Releve
    bool                   MedicalHandleHumanForCollection        ( PHY_MedicalHumanState& humanState );     // Ramassage
    PHY_RoleInterface_Medical*  MedicalReserveForSorting          ( PHY_MedicalCollectionAmbulance& ambulance );
    bool                   MedicalHandleHumanForHealing           ( PHY_MedicalHumanState& humanState );
    bool                   MedicalCanCollectionAmbulanceGo        ( const PHY_MedicalCollectionAmbulance& ambulance ) const;

    //$$$ A FACTORISER AVEC LES FONCTION CI DESSUS
    PHY_RoleInterface_Medical* MedicalFindAlternativeEvacuationHandler( const PHY_MedicalHumanState& humanState );
    PHY_RoleInterface_Medical* MedicalFindAlternativeCollectionHandler( const PHY_MedicalHumanState& humanState );
    PHY_RoleInterface_Medical* MedicalFindAlternativeSortingHandler   ( const PHY_MedicalHumanState& humanState );
    PHY_RoleInterface_Medical* MedicalFindAlternativeHealingHandler   ( const PHY_MedicalHumanState& humanState );
    //@}

    //! @name Supply
    //@{
    virtual void           SupplyHandleRequest                ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual bool           SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const;
    virtual bool           SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_ComposanteTypePion& transporterType ) const;
    virtual bool           SupplyHasStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual Stock          SupplyGetStock                     ( const PHY_DotationCategory& dotationCategory, double quantity ) const;
    virtual bool           SupplyReturnStock                  ( const PHY_DotationCategory& dotationCategory, double quantity ) const;
    virtual MIL_AgentPion* SupplyCreateConvoyPion             ( const MIL_AgentTypePion& type, const boost::shared_ptr< logistic::SupplyConvoyReal_ABC >& convoy );
    virtual void           SupplyDestroyConvoyPion            ( MIL_AgentPion& convoyPion );

    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );

    virtual const MT_Vector2D& GetPosition() const;
    virtual       bool         BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const;

    bool OnReceiveLogSupplyPushFlow( const sword::PushFlowParameters& parameters, const AutomateFactory_ABC& automateResolver );
    boost::optional< unsigned int > CreateDotationRequest( const PHY_DotationCategory& dotation, double quantity, const MIL_Automate& recipient,
                                                           unsigned int requester );
    boost::optional< unsigned int > CreateStockRequest( const PHY_DotationCategory& dotation, double quantity, const MIL_AutomateLOG& recipient,
                                                        unsigned int requester );

    void ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion );
    void ResetConsignsForProvider( const MIL_Agent_ABC& pion );
    MIL_Automate*  GetStockAutomat                    ( const PHY_DotationCategory& dotationCategory, bool& deployed );

    bool FinishAllHandlingsSuccessfullyWithoutDelay();
    //@}

    //! @name Funeral
    //@{
    virtual void AddSupplyConvoysObserver   ( logistic::SupplyConvoysObserver_ABC& observer );
    virtual void RemoveSupplyConvoysObserver( logistic::SupplyConvoysObserver_ABC& observer );

    virtual const logistic::FuneralPackagingResource* FuneralGetNextPackagingResource( const logistic::FuneralPackagingResource* currentPackaging );
    //@}

    virtual void NotifyLinkAdded( const logistic::LogisticLink_ABC& link ) const;
    virtual void NotifyLinkRemoved( const logistic::LogisticLink_ABC& link ) const;

    //! @name Quotas
    //@{
    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const;
    virtual void NotifyQuotaExceeded( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) const;
    //@}

    //! @name Network
    //@{
    template< typename Message >
    bool SendChangedState( Message& message ) const
    {
        bool modified = maintenanceManualModified_;
        if( maintenanceManualModified_ )
            message().set_log_maintenance_manual( maintenanceManual_ );
        modified |= supplyManualModified_;
        if( supplyManualModified_ )
            message().set_log_supply_manual( supplyManual_ );
        SendChangedState();
        return modified;
    }
    template< typename Message >
    void SendFullState( Message& message ) const
    {
        message().set_log_maintenance_manual( maintenanceManual_ );
        message().set_log_supply_manual( supplyManual_ );
        SendFullState();
    }
    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    virtual void Serialize( sword::ParentEntity& message ) const;
    //@}

protected:
    MIL_AutomateLOG( const PHY_LogisticLevel& level );

private:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< logistic::SupplyConsign_ABC > >      T_SupplyConsigns;
    typedef std::list< boost::shared_ptr< logistic::SupplyRequestContainer > > T_SupplyRequests;
    typedef std::list< boost::shared_ptr< logistic::SupplyRequest_ABC > >      T_MagicSupplyRequests;
    //@}

public:
    //! @name Tools
    //@{
    void SendFullState   () const;
    void SendChangedState() const;
    template< typename T > void Visit( T& visitor ) const;
    //@}

private:
    MIL_Automate*      pAssociatedAutomate_;
    MIL_Formation*     pAssociatedFormation_;
    const PHY_LogisticLevel* pLogLevel_;

    std::unique_ptr< logistic::LogisticHierarchy > pLogisticHierarchy_;

    // Supply
    T_SupplyConsigns supplyConsigns_;
    T_SupplyRequests supplyRequests_; // Pushed flows
    T_MagicSupplyRequests magicSupplyRequests_;
    std::vector< logistic::SupplyConvoysObserver_ABC* > supplyConvoysObserver_;

    bool maintenanceManual_;
    bool maintenanceManualModified_;
    bool supplyManual_;
    bool supplyManualModified_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AutomateLOG* pion, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AutomateLOG* pion, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_AutomateLOG )

#endif // __MIL_AutomateLOG_h_
