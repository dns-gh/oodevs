// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentTypePionLOG_ABC.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:30 $
// $Revision: 9 $
// $Workfile: MIL_AgentTypePionLOG_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOG_ABC.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyLoad.h"
#include "Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoyUnload.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::MIL_AgentTypePionLOG_ABC( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::~MIL_AgentTypePionLOG_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOG_ABC::RegisterFunctions( directia::Brain& brain, MIL_AgentPion& agent ) const
{
    brain.RegisterFunction( "DEC_Maintenance_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMaintenanceEnableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Maintenance_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMaintenanceDisableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPriorites", 
        boost::function< void( const std::vector< const PHY_ComposanteTypePion* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangePriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPrioritesTactiques", 
        boost::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerRegimeTravail", 
        boost::function< void( int ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangeWorkRate, boost::ref( agent ), _1 ) ) );

    brain.RegisterFunction( "DEC_Sante_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMedicalEnableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMedicalDisableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_ActiverFonctionTri",
        boost::bind( &DEC_LogisticFunctions::PionMedicalEnableSortingFunction, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverFonctionTri",
        boost::bind( &DEC_LogisticFunctions::PionMedicalDisableSortingFunction, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_ActiverFonctionSoin",
        boost::bind( &DEC_LogisticFunctions::PionMedicalEnableHealingFunction, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_DesactiverFonctionSoin",
        boost::bind( &DEC_LogisticFunctions::PionMedicalDisableHealingFunction, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Sante_ChangerPriorites", 
        boost::function< void( const std::vector< const PHY_HumanWound* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMedicalChangePriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Sante_ChangerPrioritesTactiques", 
        boost::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities, boost::ref( agent ), _1 ) ) );

    brain.RegisterFunction( "DEC_Ravitaillement_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionSupplyEnableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionSupplyDisableSystem, boost::ref( agent ) ) );
        
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_ChargementEffectue",
        boost::bind( &DEC_LogisticFunctions::ConvoyIsLoadingDone, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_DechargementEffectue",
        boost::bind( &DEC_LogisticFunctions::ConvoyIsUnloadingDone, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_AutomateRavitaillant",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetSupplyingAutomate, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_AutomateConvoyant",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetConvoyingAutomate, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_AutomateRavitaille",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetSuppliedAutomate, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_FinMission",
        boost::bind( &DEC_LogisticFunctions::ConvoyEndMission, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_StartCharger",         &DEC_ActionFunctions  ::StartAction< PHY_ActionConvoyLoad   > );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_StartDecharger",       &DEC_ActionFunctions  ::StartAction< PHY_ActionConvoyUnload > );
}

