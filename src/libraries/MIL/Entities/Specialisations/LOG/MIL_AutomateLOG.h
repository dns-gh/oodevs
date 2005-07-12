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

#include "Entities/Automates/MIL_Automate.h"

class MIL_AutomateTypeLOG;
class PHY_MaintenanceComposanteState;
class PHY_MedicalHumanState;
class PHY_Human;
class PHY_MedicalCollectionAmbulance;
class PHY_RolePion_Medical;
class PHY_Dotation;
class PHY_RolePion_Supply;
class PHY_DotationCategory;
class PHY_DotationType;
class PHY_SupplyDotationState;
class PHY_SupplyStockState;
class PHY_SupplyStockRequestContainer;
class PHY_SupplyDotationRequestContainer;
class PHY_SupplyConsign_ABC;

// =============================================================================
// @class  MIL_AutomateLOG
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateLOG : public MIL_Automate
{
    MT_COPYNOTALLOWED( MIL_AutomateLOG )

public:
    struct sDotationQuota
    {
        MT_Float rQuota_;
        MT_Float rQuotaThreshold_;
    };
    
public:
             MIL_AutomateLOG( const MIL_AutomateTypeLOG& type, uint nID, MIL_InputArchive& archive );
             MIL_AutomateLOG();
    virtual ~MIL_AutomateLOG();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Initialize
    //@{
    virtual void ReadLogisticHierarchy( MIL_InputArchive& archive );
    //@}

    //! @name Operations
    //@{
    virtual void UpdateNetwork() const;
    virtual void UpdateState  ();
    virtual void Clean        ();
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG* GetMaintenanceSuperior() const;
    MIL_AutomateLOG* GetMedicalSuperior    () const;
    MIL_AutomateLOG* GetSupplySuperior     () const;
    //@}

    //! @name Maintenance
    //@{
    PHY_MaintenanceComposanteState* MaintenanceHandleComposanteForTransport( MIL_AgentPion& pion, PHY_ComposantePion& composante );
    bool                            MaintenanceHandleComposanteForTransport( PHY_MaintenanceComposanteState& composanteState );
    bool                            MaintenanceHandleComposanteForRepair   ( PHY_MaintenanceComposanteState& composanteState );
    //@}

    //! @name Medical
    //@{
    PHY_MedicalHumanState* MedicalHandleHumanForEvacuation( MIL_AgentPion& pion, PHY_Human& human ); // Releve
    bool                   MedicalHandleHumanForCollection( PHY_MedicalHumanState& humanState );     // Ramassage    
    PHY_RolePion_Medical*  MedicalReserveForSorting       ( PHY_MedicalCollectionAmbulance& ambulance );
    bool                   MedicalHandleHumanForHealing   ( PHY_MedicalHumanState& humanState );
    bool                   MedicalCanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& ambulance ) const;
    //@}

    //! @name Supply
    //@{
    void                     SupplyHandleRequest                ( PHY_SupplyDotationState& supplyDotationState );
    void                     SupplyHandleRequest                ( PHY_SupplyStockState&    supplyStockState    );
    PHY_RolePion_Supply*     SupplyGetStockPion                 ( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const;
    bool                     SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion ) const;
    bool                     SupplyGetAvailableConvoyCommander  ( PHY_ComposantePion*& pConvoyCommander  , MIL_AgentPion*& pConvoyCommanderPion   ) const;
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
    virtual void OnReceiveMsgChangeLogisticLinks  ( ASN1T_MsgChangeLiensLogistiques&        msg, MIL_MOSContextID nCtx );
    virtual void OnReceiveMsgLogSupplyChangeQuotas( ASN1T_MsgLogRavitaillementChangeQuotas& msg, MIL_MOSContextID nCtx );
    virtual void OnReceiveMsgLogSupplyPushFlow    ( ASN1T_MsgLogRavitaillementPousserFlux&  msg, MIL_MOSContextID nCtx );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, sDotationQuota > T_DotationQuotaMap;
    typedef T_DotationQuotaMap::iterator                            IT_DotationQuotaMap;
    typedef T_DotationQuotaMap::const_iterator                      CIT_DotationQuotaMap;

    typedef std::list< PHY_SupplyConsign_ABC* >  T_SupplyConsignList;
    typedef T_SupplyConsignList::iterator        IT_SupplyConsignList;
    typedef T_SupplyConsignList::const_iterator  CIT_SupplyConsignList;
    //@}

private:
    //! @name Tools
    //@{
    MIL_AutomateLOG* GetLogisticAutomate        ( uint nID );
    void             ReadMaintenanceData        ( MIL_InputArchive& archive );
    void             ReadMedicalData            ( MIL_InputArchive& archive );
    void             ReadSupplyData             ( MIL_InputArchive& archive );
    void             ReadDotationQuotaCategories( MIL_InputArchive& archive, const PHY_DotationType& dotationType );
    void             SendQuotas                 () const;
    //@}

    //! @name Network tools
    //@{
    virtual void WriteCreationMsg( NET_ASN_MsgAutomateCreation& asnMsg ) const;
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
    PHY_SupplyStockState* pStockSupplyState_;
};

#include "MIL_AutomateLOG.inl"

#endif // __MIL_AutomateLOG_h_

