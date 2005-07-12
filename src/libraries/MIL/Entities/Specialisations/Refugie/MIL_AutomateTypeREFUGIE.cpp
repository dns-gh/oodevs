// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AutomateTypeREFUGIE.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 17:30 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeREFUGIE.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AutomateTypeREFUGIE.h"
#include "Decision/DEC_ModelAutomate.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeREFUGIE::MIL_AutomateTypeREFUGIE( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AutomateType( strName, archive )
{
    if( !GetModel().IsMissionAvailable( MIL_AutomateMissionType::GetMoveToRefugeeCampMissionType() ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Mission 'SeRendreVersCampRefugies' not specified/available in model", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeREFUGIE::~MIL_AutomateTypeREFUGIE()
{

}

