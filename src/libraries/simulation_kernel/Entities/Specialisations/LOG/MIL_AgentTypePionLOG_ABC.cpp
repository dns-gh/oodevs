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
#include "MIL_AgentPionLOG_ABC.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::MIL_AgentTypePionLOG_ABC( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
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

MIL_AgentPion* MIL_AgentTypePionLOG_ABC::InstanciatePion(
    MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const
{
    return new MIL_AgentPionLOG_ABC( *this, controller, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOG_ABC::RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& agent ) const
{
    brain.RegisterFunction( "DEC_Maintenance_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMaintenanceEnableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Maintenance_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionMaintenanceDisableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPriorites",
        std::function< void( const std::vector< const PHY_ComposanteTypePion* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangePriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerPrioritesTactiques",
        std::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Maintenance_ChangerRegimeTravail",
        std::function< void( int ) >( boost::bind( &DEC_LogisticFunctions::PionMaintenanceChangeWorkRate, boost::ref( agent ), _1 ) ) );

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
        std::function< void( const std::vector< const PHY_HumanWound* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMedicalChangePriorities, boost::ref( agent ), _1 ) ) );
    brain.RegisterFunction( "DEC_Sante_ChangerPrioritesTactiques",
        std::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities, boost::ref( agent ), _1 ) ) );

    brain.RegisterFunction( "DEC_Ravitaillement_ActiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionSupplyEnableSystem, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_DesactiverChaine",
        boost::bind( &DEC_LogisticFunctions::PionSupplyDisableSystem, boost::ref( agent ) ) );

    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue",
        boost::bind( &DEC_LogisticFunctions::ConvoyNotifyMovedToSupplier, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue",
        boost::bind( &DEC_LogisticFunctions::ConvoyNotifyMovedToTransportersProvider, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue",
        boost::bind( &DEC_LogisticFunctions::ConvoyNotifyMovedToSupplyRecipient, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_FinMission",
        boost::bind( &DEC_LogisticFunctions::ConvoyEndMission, boost::ref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_ActionCourante",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetCurrentAction, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_DestinataireCourant",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetCurrentSupplyRecipient, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_Ravitailleur",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetSupplier, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_Transporteur",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetTransportersProvider, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination",
        boost::bind( &DEC_LogisticFunctions::ConvoyGetPathToNextDestination, boost::cref( agent ) ) );
    brain.RegisterFunction( "DEC_Ravitaillement_Convoi_EstFluxPousse",
        boost::bind( &DEC_LogisticFunctions::ConvoyIsPushedFlow, boost::cref( agent ) ) );
}
