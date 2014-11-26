//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_LogisticFunctions.h $
// $Author: Jvt $
// $Modtime: 12/05/05 14:34 $
// $Revision: 10 $
// $Workfile: DEC_LogisticFunctions.h $
//
//*****************************************************************************

#ifndef __DEC_LogisticFunctions_h_
#define __DEC_LogisticFunctions_h_

#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class DEC_ResourceNetwork;
class PHY_ComposanteTypePion;
class PHY_HumanWound;

// =============================================================================
// Created: JVT 04-05-18
// =============================================================================
class DEC_LogisticFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Maintenance
    //@{
    static void MaintenanceEnableSystem( DEC_Decision_ABC* caller );
    static void MaintenanceDisableSystem( DEC_Decision_ABC* caller );
    static void MaintenanceChangePriorities( DEC_Decision_ABC* caller, const std::vector< const PHY_ComposanteTypePion* >& priorities );
    static std::vector< const PHY_ComposanteTypePion* > GetAutomateMaintenancePriorities( const DEC_Decision_ABC* callerAutomate );
    static void MaintenanceChangeTacticalPriorities( DEC_Decision_ABC* caller, const std::vector< const DEC_Decision_ABC* >& priorities );
    static std::vector< const DEC_Decision_ABC* > GetAutomateMaintenanceTacticalPriorities( const DEC_Decision_ABC* callerAutomate );
    static void MaintenanceChangeWorkRate( DEC_Decision_ABC* caller, int workRate );
    static int GetAutomateMaintenanceWorkRate( const DEC_Decision_ABC* callerAutomate );
    //@}

    //! @name Medical
    //@{
    static void MedicalEnableSystem( DEC_Decision_ABC* caller );
    static void MedicalDisableSystem( DEC_Decision_ABC* caller );
    static void MedicalEnableSortingFunction ( DEC_Decision_ABC* caller );
    static void MedicalDisableSortingFunction( DEC_Decision_ABC* caller);
    static void MedicalEnableHealingFunction( DEC_Decision_ABC* caller );
    static void MedicalDisableHealingFunction( DEC_Decision_ABC* caller );
    static void MedicalChangePriorities( DEC_Decision_ABC* caller, const std::vector< const PHY_HumanWound* >& priorities );
    static std::vector< const PHY_HumanWound* > GetAutomateMedicalPriorities( const DEC_Decision_ABC* callerAutomate );
    static void MedicalChangeTacticalPriorities( DEC_Decision_ABC* caller, const std::vector< const DEC_Decision_ABC* >& priorities );
    static std::vector< const DEC_Decision_ABC* > GetAutomateMedicalTacticalPriorities( const DEC_Decision_ABC* callerAutomate );

    static void EvacuateWoundedHumansToTC2( DEC_Decision_ABC* pPionWounded, DEC_Decision_ABC* pDecTC2 );
    static bool HasWoundedHumansToEvacuate( DEC_Decision_ABC* pPion );
    static void ForbidWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion );
    static void AllowWoundedHumansAutoEvacuation( DEC_Decision_ABC* pPion );
    //@}

    //! @name Supply
    //@{
    static void SupplyEnableSystem( DEC_Decision_ABC* caller );
    static void SupplyDisableSystem( DEC_Decision_ABC* caller );

    static void ConvoyNotifyMovedToSupplier( DEC_Decision_ABC* callerAgent );
    static void ConvoyNotifyMovedToTransportersProvider( DEC_Decision_ABC* callerAgent );
    static void ConvoyNotifyMovedToSupplyRecipient( DEC_Decision_ABC* callerAgent );
    static void ConvoyEndMission( DEC_Decision_ABC* callerAgent );
    static int  ConvoyGetCurrentAction( const DEC_Decision_ABC* callerAgent );
    static DEC_Decision_ABC* ConvoyGetCurrentSupplyRecipient( const DEC_Decision_ABC* callerAgent );
    static DEC_Decision_ABC* ConvoyGetTransportersProvider  ( const DEC_Decision_ABC* callerAgent );
    static DEC_Decision_ABC* ConvoyGetSupplier              ( const DEC_Decision_ABC* callerAgent );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ConvoyGetPathToNextDestination( const DEC_Decision_ABC* callerAgent );
    static bool ConvoyIsPushedFlow( const DEC_Decision_ABC* callerAgent );

    static void RequestSupply( DEC_Decision_ABC* caller );

    static void ChangeDotationsValueUsingTC2( DEC_Decision_ABC* callerAgent, int dotationType, const double rCapacityFactor, int ammoDotationClassId );
    static void EnforceAviationResources( DEC_Decision_ABC* callerAgent, int aviationRange );

    static void ConnectToResourceNode( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    static void DisconnectFromResourceNode( DEC_Decision_ABC* callerAgent );
    //@}

    //! @name Misc
    //@{
    static DEC_Decision_ABC* PionGetPcTC2( const DEC_Decision_ABC* );
    static DEC_Decision_ABC* AutomateGetPcTC2( const DEC_Decision_ABC* );

    static void UndoLendCollectionComposantes( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pTarget, unsigned int nNbrToGetBack );
    static void UndoLendHaulerComposantes( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pTarget, unsigned int nNbrToGetBack );
    static void UndoLendSpecificComposantes( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pTarget, PHY_ComposanteTypePion* type, unsigned int nNbrToGetBack );
    //@}
};

#endif // __DEC_LogisticFunctions_h_
