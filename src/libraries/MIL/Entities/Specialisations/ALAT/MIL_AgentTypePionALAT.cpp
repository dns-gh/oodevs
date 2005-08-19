// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Alat/MIL_AgentTypePionALAT.cpp $
// $Author: Jvt $
// $Modtime: 24/05/05 10:17 $
// $Revision: 19 $
// $Workfile: MIL_AgentTypePionALAT.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_AgentTypePionALAT.h"

#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_AgentFunctions.h"
#include "Decision/Functions/DEC_PerceptionFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::MIL_AgentTypePionALAT( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePion( strName, archive )
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRecoAlat                 , "DEC_ALAT_ActiverReconnaissance"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRecoAlat                , "DEC_ALAT_DesactiverReconnaissance"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::HasNoDelayedPeceptions         , "DEC_ALAT_ReconnaissanceNonVuTerminee"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableSurveillanceLocalisation , "DEC_Perception_ActiverSurveillance"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableSurveillanceLocalisation, "DEC_Perception_DesactiverSurveillance" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::~MIL_AgentTypePionALAT()
{

}

