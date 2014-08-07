// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateTypeLOG.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:59 $
// $Revision: 4 $
// $Workfile: MIL_AutomateTypeLOG.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateTypeLOG.h"
#include "MIL_AutomateLOG.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::MIL_AutomateTypeLOG( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::~MIL_AutomateTypeLOG()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::IsLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateTypeLOG::IsLogistic() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID,
                                                        MissionController_ABC& controller,
                                                        MIL_Entity_ABC& parent,
                                                        xml::xistream& xis,
                                                        unsigned int gcPause,
                                                        unsigned int gcMult,
                                                        sword::DEC_Logger* logger ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, xis, gcPause, gcMult, logger );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID,
                                                        MissionController_ABC& controller,
                                                        MIL_Entity_ABC& parent,
                                                        unsigned int knowledgeGroup,
                                                        const std::string& name,
                                                        unsigned int gcPause,
                                                        unsigned int gcMult,
                                                        sword::DEC_Logger* logger,
                                                        unsigned int context,
                                                        const MIL_DictionaryExtensions& extensions ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, knowledgeGroup, name, gcPause, gcMult, logger, context, extensions );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::Create
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateTypeLOG::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeLOG( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AutomateTypeLOG::RegisterFunctions( sword::Brain& brain, MIL_Automate& automat ) const
{
    brain.RegisterFunction( "DEC_Maintenance_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceEnableSystem, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Maintenance_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceDisableSystem, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPriorites",
        std::function< void( const std::vector< const PHY_ComposanteTypePion* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangePriorities, boost::ref( automat ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_Priorites",
        std::function< std::vector< const PHY_ComposanteTypePion* >() >( boost::bind( &DEC_LogisticFunctions::GetAutomateMaintenancePriorities, boost::ref( automat ) ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPrioritesTactiques",
        std::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities, boost::ref( automat ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_PrioritesTactiques",
        std::function< std::vector< const DEC_Decision_ABC* >() >( boost::bind( &DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities, boost::ref( automat ) ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerRegimeTravail",
        std::function< void( int ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate, boost::ref( automat ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_RegimeTravail",
        std::function< int() >( boost::bind( &DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate, boost::ref( automat ) ) ) );

    brain.RegisterFunction( "DEC_Sante_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableSystem, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableSystem, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_ActiverFonctionTri",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverFonctionTri",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_ActiverFonctionSoin",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverFonctionSoin",
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Sante_ChangerPriorites",
        std::function< void( const std::vector< const PHY_HumanWound* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMedicalChangePriorities, boost::ref( automat ), _1 ) ) );
    brain.RegisterFunction( "DEC_Sante_Priorites",
        std::function< std::vector< const PHY_HumanWound* >() >( boost::bind( &DEC_LogisticFunctions::GetAutomateMedicalPriorities, boost::ref( automat ) ) ) );
    brain.RegisterFunction( "DEC_Sante_ChangerPrioritesTactiques",
        std::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities, boost::ref( automat ), _1 ) ) );
    brain.RegisterFunction( "DEC_Sante_PrioritesTactiques",
        std::function< std::vector< const DEC_Decision_ABC* >() >( boost::bind( &DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities, boost::ref( automat ) ) ) );

    brain.RegisterFunction( "DEC_Ravitaillement_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateSupplyEnableSystem, boost::ref( automat ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::AutomateSupplyDisableSystem, boost::ref( automat ) ) );
}

