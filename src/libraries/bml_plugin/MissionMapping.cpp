// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionMapping.h"
#include "clients_kernel/MissionType.h"

namespace bml
{

    // -----------------------------------------------------------------------------
    // Name: GetMissionNameFromCode
    // Created: SBO 2008-05-23
    // -----------------------------------------------------------------------------
    std::string GetMissionNameFromCode( const std::string& code )
    {
        if( code == "ATTACK" )
            return "ABC GTIA Attaquer";
        if( code == "BLOCK" )
            return "ABC GTIA DonnerCoupArret";
        if( code == "DELAY" )
            return "ABC GTIA Freiner";
        throw std::runtime_error( __FUNCTION__ ": Unsupported mission" );
    }

    // -----------------------------------------------------------------------------
    // Name: GetCodeFromMissionId
    // Created: SBO 2008-05-23
    // -----------------------------------------------------------------------------
    std::string GetCodeFromMissionId( const kernel::Resolver_ABC< kernel::MissionType >& missions, unsigned int id )
    {
        const kernel::MissionType& mission = missions.Get( id );
        if( mission.GetName() == "ABC GTIA Freiner" )
            return "ATTACK";
        if( mission.GetName() == "ABC GTIA DonnerCoupArret" )
            return "BLOCK";
        if( mission.GetName() == "ABC GTIA Freiner" )
            return "DELAY";
        throw std::runtime_error( __FUNCTION__ ": Unsupported mission" );
    }

    // -----------------------------------------------------------------------------
    // Name: GetParameterTypeFromCode
    // Created: SBO 2008-05-23
    // -----------------------------------------------------------------------------
    std::string GetParameterTypeFromCode( const std::string& code )
    {
        if( code == "PHLINE" )
            return "phaselinelist";
        if( code == "BDYOR" )
            return "limit";
        if( code == "MAXIS" )
            return "direction";
        if( code == "OBJA" )
            return "polygon";
        throw std::runtime_error( __FUNCTION__ ": Unsupported parameter type");
    }
}
