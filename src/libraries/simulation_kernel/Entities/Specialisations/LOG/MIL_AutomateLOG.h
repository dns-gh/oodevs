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

#include "Entities/Automates/MIL_Automate.h"

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_DataBase;
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
class PHY_SupplyDotationState;
class PHY_SupplyStockState;
class PHY_SupplyStockRequestContainer;
class PHY_SupplyDotationRequestContainer;
class PHY_SupplyConsign_ABC;
class PHY_ComposantePion;

template < typename T > class PHY_ActionLogistic;

// =============================================================================
// @class  MIL_AutomateLOG
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateLOG : public MIL_Automate
{

public:
    struct sDotationQuota
    {
        MT_Float rQuota_;
        MT_Float rQuotaThreshold_;
    };
    
public:
             MIL_AutomateLOG( const MIL_AutomateTypeLOG& type, unsigned int nID, MIL_Formation& parent, xml::xistream& xis, DEC_DataBase& database );
             MIL_AutomateLOG( const MIL_AutomateTypeLOG& type, unsigned int nID, MIL_Automate&  parent, xml::xistream& xis, DEC_DataBase& database );
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

    virtual void UpdateNetwork() const;
    virtual void UpdateState  ();
    virtual void Clean        ();
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG* GetMaintenanceSuperior() const;
    MIL_AutomateLOG* GetMedicalSuperior    () const;
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
    void                     SupplyHandleRequest                ( PHY_SupplyDotationState& supplyDotationState );
    void                     SupplyHandleRequest                ( PHY_SupplyStockState&    supplyStockState    );
    PHY_RoleInterface_Supply* SupplyGetStockPion                ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const;
    bool                     SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const;
    MT_Float                 SupplyGetStock                     ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const;
    bool                     SupplyReturnStock                  ( const PHY_DotationCategory& dotationCategory, MT_Float rReturnedValue  ) const;

    void                     NotifyStockSupplyNeeded            ( const PHY_DotationCategory& dotationCategory );
    void                     NotifyStockSupplied                ( const PHY_SupplyStockState& supplyState );
    void                     NotifyStockSupplyCanceled          ( const PHY_SupplyStockState& supplyState );
    //@}

    //! @name Quotas
    //@{
    MT_Float GetQuota    ( const PHY_DotationCategory& dotationCategory ) const;
    void     ConsumeQuota( const PHY_DotationCategory& dotationCategory, MT_Float rQuotaConsumed );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState                    () const;
    virtual void OnReceiveMsgChangeLogisticLinks  ( const Common::MsgAutomatChangeLogisticLinks& msg );
    virtual void OnReceiveMsgLogSupplyChangeQuotas( const MsgsClientToSim::MsgLogSupplyChangeQuotas& msg );
    virtual void OnReceiveMsgLogSupplyPushFlow    ( const MsgsClientToSim::MsgLogSupplyPushFlow& msg );
    //@}

protected:
    MIL_AutomateLOG( const MIL_AutomateTypeLOG& type, unsigned int nID);

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, sDotationQuota > T_DotationQuotaMap;
    typedef T_DotationQuotaMap::iterator                            IT_DotationQuotaMap;
    typedef T_DotationQuotaMap::const_iterator                      CIT_DotationQuotaMap;

    typedef std::list< PHY_SupplyConsign_ABC* >  T_SupplyConsignList;
    typedef T_SupplyConsignList::iterator        IT_SupplyConsignList;
    typedef T_SupplyConsignList::const_iterator  CIT_SupplyConsignList;

    typedef std::set< PHY_SupplyStockState* >     T_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::iterator       IT_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::const_iterator CIT_SupplyStockStateSet;
    //@}

private:
    //! @name Tools
    //@{
    MIL_AutomateLOG* GetLogisticAutomate   ( unsigned int nID );
    void             SendQuotas            () const;
    bool             IsSupplyInProgress    ( const PHY_DotationCategory& dotationCategory ) const;
    void             RemoveSupplyStockState( const PHY_SupplyStockState& supplyState );
    //@}

    //! @name Tools
    //@{
    virtual void SendLogisticLinks    () const;
    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}
    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}
    
private:
    MIL_AutomateLOG* pMaintenanceSuperior_;
    MIL_AutomateLOG* pMedicalSuperior_;
    MIL_AutomateLOG* pSupplySuperior_;

    // Supply   
    T_SupplyConsignList supplyConsigns_;

    T_DotationQuotaMap    stockQuotas_;
    bool                  bQuotasHaveChanged_;
    bool                  bStockSupplyNeeded_;
    PHY_SupplyStockState* pExplicitStockSupplyState_;
    T_SupplyStockStateSet pushedFlowsSupplyStates_;

    unsigned int                  nTickRcStockSupplyQuerySent_;

    PHY_ActionLogistic< MIL_AutomateLOG >* pLogisticAction_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AutomateLOG* pion, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AutomateLOG* pion, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( MIL_AutomateLOG )

#endif // __MIL_AutomateLOG_h_

