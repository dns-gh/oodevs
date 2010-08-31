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
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_Tools.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <directia/brain/Brain.h>

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
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID, MIL_Formation& parent, xml::xistream& xis, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult ) const
{
    return *new MIL_AutomateLOG( *this, nID, parent, xis, database, gcPause, gcMult );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID, MIL_Automate& parent, xml::xistream& xis, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult ) const
{
    return *new MIL_AutomateLOG( *this, nID, parent, xis, database, gcPause, gcMult );
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
void MIL_AutomateTypeLOG::RegisterFunctions( directia::brain::Brain& brain, MIL_Automate& automat ) const
{
    brain[ "DEC_Maintenance_ActiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceEnableSystem, boost::ref( automat ) );
    brain[ "DEC_Maintenance_DesactiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceDisableSystem, boost::ref( automat ) );
    brain[ "DEC_Maintenance_ChangerPriorites" ] =
        boost::function< void( const std::vector< const PHY_ComposanteTypePion* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangePriorities, boost::ref( automat ), _1 ) );
    brain[ "DEC_Maintenance_ChangerPrioritesTactiques" ] =
        boost::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities, boost::ref( automat ), _1 ) );
    brain[ "DEC_Maintenance_ChangerRegimeTravail" ] =
        boost::function< void( int ) >( boost::bind( &DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate, boost::ref( automat ), _1 ) );

    brain[ "DEC_Sante_ActiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableSystem, boost::ref( automat ) );
    brain[ "DEC_Sante_DesactiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableSystem, boost::ref( automat ) );
    brain[ "DEC_Sante_ActiverFonctionTri" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction, boost::ref( automat ) );
    brain[ "DEC_Sante_DesactiverFonctionTri" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction, boost::ref( automat ) );
    brain[ "DEC_Sante_ActiverFonctionSoin" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction, boost::ref( automat ) );
    brain[ "DEC_Sante_DesactiverFonctionSoin" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction, boost::ref( automat ) );
    brain[ "DEC_Sante_ChangerPriorites" ] =
        boost::function< void( const std::vector< const PHY_HumanWound* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMedicalChangePriorities, boost::ref( automat ), _1 ) );
    brain[ "DEC_Sante_ChangerPrioritesTactiques" ] =
        boost::function< void( const std::vector< const DEC_Decision_ABC* >& ) >( boost::bind( &DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities, boost::ref( automat ), _1 ) );

    brain[ "DEC_Ravitaillement_ActiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateSupplyEnableSystem, boost::ref( automat ) );
    brain[ "DEC_Ravitaillement_DesactiverChaine" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateSupplyDisableSystem, boost::ref( automat ) );
}

