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

#include "MIL_pch.h"

#include "MIL_AgentTypePionLOG_ABC.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoySupply.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendComposante.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::MIL_AgentTypePionLOG_ABC( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePion( strName, archive )
{ 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceEnableSystem            , "DEC_Maintenance_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceDisableSystem           , "DEC_Maintenance_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangePriorities        , "DEC_Maintenance_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangeTacticalPriorities, "DEC_Maintenance_ChangerPrioritesTactiques" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMaintenanceChangeWorkTime          , "DEC_Maintenance_ChangerTempsBordee"        );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalEnableSystem                , "DEC_Sante_ActiverChaine"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalDisableSystem               , "DEC_Sante_DesactiverChaine"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalChangePriorities            , "DEC_Sante_ChangerPriorites"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalChangeTacticalPriorities    , "DEC_Sante_ChangerPrioritesTactiques" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionMedicalChangeWorkTime              , "DEC_Sante_ChangerTempsBordee"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionLendComposante >, "DEC_Sante_StartPreterVSRAM"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::UndoLendCollectionComposantes          , "DEC_Sante_RecupererVSRAM"            );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionSupplyEnableSystem , "DEC_Ravitaillement_ActiverChaine"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionSupplyDisableSystem, "DEC_Ravitaillement_DesactiverChaine"   );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyIsSupplyDone                   , "DEC_Ravitaillement_Convoi_RavitaillementEffectue" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyGetSuppliedAutomate            , "DEC_Ravitaillement_Convoi_AutomateARavitailler"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ConvoyEndMission                     , "DEC_Ravitaillement_Convoi_FinMission"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionConvoySupply >, "DEC_Ravitaillement_Convoi_StartRavitailler"       );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOG_ABC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOG_ABC::~MIL_AgentTypePionLOG_ABC()
{

}

