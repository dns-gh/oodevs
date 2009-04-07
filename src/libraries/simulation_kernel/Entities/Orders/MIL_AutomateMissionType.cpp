// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateMissionType.h"
#include <xeumeuleu/xml.h>



MIL_MissionType_ABC::T_MissionIDMap   MIL_AutomateMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionIDMap   MIL_AutomateMissionType::missionDiaIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_AutomateMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_AutomateMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_MissionType_ABC::ReadMission< MIL_AutomateMissionType >( xis, missionIDs_, &missionDiaIDs_, missionNames_ );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_AutomateMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing automate mission types" );

    LoadingWrapper loader;
    
    xis >> xml::start( "missions" )
            >> xml::start( "automats" )
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
            >> xml::end()
        >> xml::end();
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::MIL_AutomateMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIAMrtBehavior_ ()
    , strDIACdtBehavior_ ()
{
    xis >> xml::attribute( "mrt-dia-behavior", strDIAMrtBehavior_ )
        >> xml::attribute( "cdt-dia-behavior", strDIACdtBehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::~MIL_AutomateMissionType()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_AutomateMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    return IsMissionAvailableForModel( model, strDIAMrtBehavior_ ) && IsMissionAvailableForModel( model, strDIACdtBehavior_ );
}
