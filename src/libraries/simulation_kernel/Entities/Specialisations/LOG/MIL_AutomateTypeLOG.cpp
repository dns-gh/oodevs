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
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/DEC_Tools.h"

#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::MIL_AutomateTypeLOG( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMaintenanceEnableSystem            , "DEC_Maintenance_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMaintenanceDisableSystem           , "DEC_Maintenance_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMaintenanceChangePriorities        , "DEC_Maintenance_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMaintenanceChangeTacticalPriorities, "DEC_Maintenance_ChangerPrioritesTactiques" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMaintenanceChangeWorkRate          , "DEC_Maintenance_ChangerRegimeTravail"      );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalEnableSystem            , "DEC_Sante_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalDisableSystem           , "DEC_Sante_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalEnableSortingFunction   , "DEC_Sante_ActiverFonctionTri"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalDisableSortingFunction  , "DEC_Sante_DesactiverFonctionTri"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalEnableHealingFunction   , "DEC_Sante_ActiverFonctionSoin"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalDisableHealingFunction  , "DEC_Sante_DesactiverFonctionSoin"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalChangePriorities        , "DEC_Sante_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateMedicalChangeTacticalPriorities, "DEC_Sante_ChangerPrioritesTactiques" );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateSupplyEnableSystem    , "DEC_Ravitaillement_ActiverChaine"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateSupplyDisableSystem   , "DEC_Ravitaillement_DesactiverChaine"   );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::~MIL_AutomateTypeLOG()
{

}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::IsLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateTypeLOG::IsLogistic() const
{
    return true;
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( uint nID, MIL_Formation& formation, xml::xistream& xis ) const
{
    return *new MIL_AutomateLOG( *this, nID, formation, xis );
}
