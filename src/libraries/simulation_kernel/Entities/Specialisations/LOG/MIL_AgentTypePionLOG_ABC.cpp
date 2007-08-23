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
#include <functional>
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::MIL_AgentTypePionLOG_ABC( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{ 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceEnableSystem            , "DEC_Maintenance_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceDisableSystem           , "DEC_Maintenance_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangePriorities        , "DEC_Maintenance_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities, "DEC_Maintenance_ChangerPrioritesTactiques" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangeWorkRate          , "DEC_Maintenance_ChangerRegimeTravail"      );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalEnableSystem                , "DEC_Sante_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalDisableSystem               , "DEC_Sante_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalEnableSortingFunction       , "DEC_Sante_ActiverFonctionTri"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalDisableSortingFunction      , "DEC_Sante_DesactiverFonctionTri"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalEnableHealingFunction       , "DEC_Sante_ActiverFonctionSoin"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalDisableHealingFunction      , "DEC_Sante_DesactiverFonctionSoin"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalChangePriorities            , "DEC_Sante_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities    , "DEC_Sante_ChangerPrioritesTactiques" );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionSupplyEnableSystem                 , "DEC_Ravitaillement_ActiverChaine"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionSupplyDisableSystem                , "DEC_Ravitaillement_DesactiverChaine" );
        
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyIsLoadingDone                    , "DEC_Ravitaillement_Convoi_ChargementEffectue"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyIsUnloadingDone                  , "DEC_Ravitaillement_Convoi_DechargementEffectue" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyGetSupplyingAutomate             , "DEC_Ravitaillement_Convoi_AutomateRavitaillant" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyGetConvoyingAutomate             , "DEC_Ravitaillement_Convoi_AutomateConvoyant"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyGetSuppliedAutomate              , "DEC_Ravitaillement_Convoi_AutomateRavitaille"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyEndMission                       , "DEC_Ravitaillement_Convoi_FinMission"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionConvoyLoad   >  , "DEC_Ravitaillement_Convoi_StartCharger"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionConvoyUnload >  , "DEC_Ravitaillement_Convoi_StartDecharger"       );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::~MIL_AgentTypePionLOG_ABC()
{

}

