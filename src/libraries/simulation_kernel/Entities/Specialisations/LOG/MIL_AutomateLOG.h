// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateLOG.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:33 $
// $Revision: 12 $
// $Workfile: MIL_AutomateLOG.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateLOG_h_
#define __MIL_AutomateLOG_h_

#include "MIL.h"
#include "Entities/Actions/PHY_Actor.h"
#include "Entities/Specialisations/LOG/LogisticHierarchyOwner_ABC.h"
#include "Entities/Agents/Roles/Logistic/SupplySupplier_ABC.h"
#include "tools/Resolver.h"

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
}

class MIL_Agent_ABC;
class MIL_AutomateTypeLOG;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class Human_ABC;
class PHY_MedicalCollectionAmbulance;
class PHY_RoleInterface_Medical;
class PHY_Dotation;
class PHY_RoleInterface_Supply;
class PHY_DotationCategory;
class PHY_DotationType;
class PHY_ComposantePion;
class MIL_Automate;
class MIL_Formation;
class PHY_LogisticLevel;
class MIL_Army_ABC;
class MIL_AgentPion;
class MIL_AgentTypePion;
template < typename T > class PHY_ActionLogistic;

// =============================================================================
// @class  MIL_AutomateLOG
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateLOG : public PHY_Actor
                      , public logistic::LogisticHierarchyOwner_ABC
                      , public logistic::SupplySupplier_ABC
{
public:
             MIL_AutomateLOG( MIL_Formation& formation, const PHY_LogisticLevel& level);
             MIL_AutomateLOG( MIL_Automate&  automat, const PHY_LogisticLevel& level);
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
    void UpdateState   ();
    void Clean         ();
    //@}

    //! @name Accessors
    //@{
    unsigned int                      GetID() const;
    logistic::LogisticHierarchy_ABC&  GetLogisticHierarchy() const;
    MIL_Army_ABC&                     GetArmy     () const;
    virtual const MIL_AgentPion*      GetPC      () const;
    const PHY_LogisticLevel&          GetLogisticLevel() const;
    //@}

    //! @name Maintenance
    //@{
    PHY_MaintenanceComposanteState* MaintenanceHandleComposanteForTransport( MIL_Agent_ABC& pion, PHY_ComposantePion& composante );
    bool                            MaintenanceHandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState );
    bool                            MaintenanceHandleComposanteForRepair   ( PHY_MaintenanceComposanteState& composanteState );
    //@}

    //! @name Medical
    //@{
    PHY_MedicalHumanState* MedicalHandleHumanEvacuatedByThirdParty( MIL_AgentPion& pion, Human_ABC& human ); // Imex
    PHY_MedicalHumanState* MedicalHandleHumanForEvacuation        ( MIL_AgentPion& pion, Human_ABC& human ); // Releve
    bool                   MedicalHandleHumanForCollection        ( PHY_MedicalHumanState& humanState );     // Ramassage
    PHY_RoleInterface_Medical*  MedicalReserveForSorting          ( PHY_MedicalCollectionAmbulance& ambulance );
    bool                   MedicalHandleHumanForHealing           ( PHY_MedicalHumanState& humanState );
    bool                   MedicalCanCollectionAmbulanceGo        ( const PHY_MedicalCollectionAmbulance& ambulance ) const;
    //@}

    //! @name Supply
    //@{
    virtual void           SupplyHandleRequest                ( boost::shared_ptr < logistic::SupplyConsign_ABC > consign );
    virtual bool           SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const;
    virtual bool           SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_ComposanteTypePion& transporterType ) const;
    virtual bool           SupplyHasStock                     ( const PHY_DotationCategory& dotationCategory ) const;
    virtual double         SupplyGetStock                     ( const PHY_DotationCategory& dotationCategory, double quantity ) const;
    virtual bool           SupplyReturnStock                  ( const PHY_DotationCategory& dotationCategory, double quantity ) const;
    virtual MIL_AgentPion* SupplyCreateConvoyPion             ( const MIL_AgentTypePion& type, boost::shared_ptr< logistic::SupplyConvoyReal_ABC > convoy );
    virtual void           SupplyDestroyConvoyPion            ( MIL_AgentPion& convoyPion );    
    
    virtual const MT_Vector2D& GetPosition() const;
    
    void           OnReceiveLogSupplyPushFlow         ( const sword::PushFlowParameters& parameters, const tools::Resolver_ABC< MIL_Automate >& automateResolver );
    //@}

    //! @name Quotas
    //@{
    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const;
    //@}

    //! @name Network
    //@{
    void SendFullState   () const;
    void SendChangedState() const;

    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    virtual void Serialize( sword::ParentEntity& message ) const;
    //@}

protected:
    MIL_AutomateLOG( const PHY_LogisticLevel& level );

private:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr < logistic::SupplyConsign_ABC > >  T_SupplyConsigns;
    typedef std::list< boost::shared_ptr < logistic::SupplyRequestContainer > >  T_SupplyRequests;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void Visit( T& visitor ) const;
    //@}

private:
    MIL_Automate*      pAssociatedAutomate_;
    MIL_Formation*     pAssociatedFormation_;
    const PHY_LogisticLevel* pLogLevel_;

    std::auto_ptr< logistic::LogisticHierarchy > pLogisticHierarchy_;

    // Supply
    T_SupplyConsigns supplyConsigns_;
    T_SupplyRequests supplyRequests_; // Pushed flows

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AutomateLOG* pion, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AutomateLOG* pion, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( MIL_AutomateLOG )

#endif // __MIL_AutomateLOG_h_

