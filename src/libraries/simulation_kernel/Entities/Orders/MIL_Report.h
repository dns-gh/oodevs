// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Report_h_
#define __MIL_Report_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class MIL_ParameterType_ABC;
class DEC_KnowledgeResolver_ABC;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class MIL_Effect_IndirectFire;

// =============================================================================
/** @class  MIL_Report
    @brief  MIL_Report
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_Report
{
public:
    //! @name Types
    //@{
    enum E_Type
    {
        eRcTypeMessage     = 0,
        eRcTypeOperational = 1,
        eRcTypeEvent       = 2,
        eRcTypeWarning     = 3
    };

    enum E_EngineReport
    {
        eReport_ReAvailableAfterRepairation,
        eReport_EquipementRepairedInPlace,
        eReport_EquipementBackFromMaintenance,
        eReport_IndirectFireObservation,    
        eReport_StockSupplyRequest,
        eReport_DotationSupplyRequest,
        eReport_MedicalEvacuationRequest,
        eReport_EquipementEvacuationRequest,
        eReport_DotationSupplyDone,
        eReport_StockSupplyDone,
        eReport_DotationSupplyCanceled,
        eReport_StockSupplyCanceled,
        eReport_LogisticStockThresholdExceeded,
        eReport_LogisticDotationThresholdExceeded, 
        eReport_HumanBackFromMedical,
        eReport_WoundedManDeath,
        eReport_WoundedManDeathDuringTransport,
        eReport_WoundedManDeathDuringHospitalization,
        eReport_FireInForbiddenArea,
        eReport_FratricideIndirectFire,
        eReport_IndirectFireOnPopulation,
        eReport_FireOnFriendSide, 
        eReport_FiredByFriendSide,
        eReport_FireOnNeutralSide,
        eReport_FiredByNeutralSide,
        eReport_FireOnCivilian,
        eReport_FiredByCivilian,
        eReport_CommandPostDestruction,
        eReport_QuotaAlmostConsumed,
        eReport_StockCapacityExceeded,
        eReport_MissionImpossible_,
        eReport_UnitDecontaminated,
        eReport_OutOfGas,
        eReport_DifficultTerrain,
        eReport_Surrendered,
        eReport_CancelSurrender,
        eReport_MaintenanceWorkRateExceeded,
        eReport_EquipmentLoanInProgress,
        eReport_EquipmentLoanCanceled,
        eReport_EquipmentLoanDone,
        eReport_EquipmentLoanPartiallyDone,
        eReport_EquipmentLoanImpossible,
        eReport_EquipmentLoanRetrievingDone,
        eReport_EquipmentLoanRetrievingPartiallyDone,
        eReport_EquipmentLoanRetrievingImpossible,
        eReport_EquipmentLent,
        eReport_EquipmentLoanGivenBack,
        eReport_InterventionAgainstPopulationStarted,
        eReport_MineExplosionOnPopulation,
        eReport_ConfrontationWithPopulation,
        eReport_Blocked,
        eReport_Filtered,
        eReport_TerroristAttackAgainstPopulation,
        eReport_EvacuationResourcesLevelReached,
        eReport_CollectionResourcesLevelReached,
        eReport_DoctorResourcesLevelReached,
        eReport_RepairerResourcesLevelReached,
        eReport_HaulerResourcesLevelReached,
        eReport_ConvoyTransporterResourcesLevelReached,
        eNbrReport
    };
    //@}
public:
    //! @name Factory
    //@{
    static       void        Initialize( xml::xistream& xis );
    static const MIL_Report* Find      ( uint nID );
    //@}

    //! @name Operations
    //@{
    template< typename T > void Send( const T& sender, E_Type nType, const DIA_Parameters& diaParameters ) const;
    //@}

    //! @name $$ BOF
    //@{
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const PHY_ComposanteTypePion& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const PHY_DotationCategory& parameter );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, MT_Float nParam1, MT_Float nParam2 );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, int nParam );
    template< typename T > static void PostEvent( const T& receiver, E_EngineReport nReport, const MIL_Effect_IndirectFire& flyingShell );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    MIL_Report( uint nID, xml::xistream& xis );
    ~MIL_Report();
    //@}

    //! @name Tools
    //@{
    template< typename T > 
    static void PostEvent( const T& receiver, E_EngineReport nReport, DIA_Parameters& parameters );

    bool Send( uint nSenderID, E_Type nType, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const DIA_Parameters& diaParameters ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< uint, const MIL_Report* > T_ReportMap;
    typedef T_ReportMap::const_iterator         CIT_ReportMap;

    typedef std::vector< const MIL_ParameterType_ABC* > T_ParameterVector;
    typedef T_ParameterVector::const_iterator           CIT_ParameterVector;

    typedef std::vector< const std::string > T_DiaEventVector;
    //@}

private:
    uint              nID_;
    std::string       strMessage_;
    T_ParameterVector parameters_;

private:
    static T_ReportMap      reports_;
    static T_DiaEventVector diaEvents_;

private:
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadReport( xml::xistream& xis );
    void ReadParameter ( xml::xistream& xis );
    //@}
};

#include "MIL_Report.inl"

#endif // __MIL_Report_h_
