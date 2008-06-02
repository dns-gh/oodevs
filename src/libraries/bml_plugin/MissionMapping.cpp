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
	namespace
	{
		// -----------------------------------------------------------------------------
		// Name: GetMissionId
		// Created: SBO 2008-05-23
		// -----------------------------------------------------------------------------
		unsigned long GetMissionId( const kernel::Resolver_ABC< kernel::MissionType >& missions, const std::string& name1, const std::string& name2 )
		{
			kernel::Iterator< const kernel::MissionType& > it( missions.CreateIterator() );
			while( it.HasMoreElements() )
			{
				const kernel::MissionType& type = it.NextElement();
				if( type.GetName() == name1 || type.GetName() == name2 )
					return type.GetId();
			}
			throw std::runtime_error( __FUNCTION__ ": Unable to resolve Mission" );
		}
	}

    // -----------------------------------------------------------------------------
    // Name: GetMissionIdFromCode
    // Created: SBO 2008-05-23
    // -----------------------------------------------------------------------------
    unsigned long GetMissionIdFromCode( const kernel::Resolver_ABC< kernel::MissionType >& missions, const std::string& code )
    {
        if( code == "ATTACK" )
            return GetMissionId( missions, "ABC GTIA Attaquer", "Battalion - Attack" );
        if( code == "BLOCK" )
            return GetMissionId( missions, "ABC GTIA DonnerCoupArret", "Battalion - conduct a blocking action" );
        if( code == "DELAY" )
            return GetMissionId( missions, "ABC GTIA Freiner", "Battalion - delay" );
        if( code == "RECCE" )
            return GetMissionId( missions, "ABC Reconnaitre Axe", "Armor - reconnoiter an axis" );
        if( code == "RECCEF" )
            return GetMissionId( missions, "INF Reconnaitre Axe", "Infantry - reconnoiter an axis" );
        if( code == "SEIZNG" )
            return GetMissionId( missions, "INF Semparer", "Infantry - seize" );
        if( code == "HLDOFF" )
            return GetMissionId( missions, "INF Defendre Ferme", "Infantry - defend (until the end)" );
        if( code == "MOVE" )
            return GetMissionId( missions, "Faire Mouvement", "move" );
        if( code == "FOLASS" )
            return GetMissionId( missions, "ABC Soutenir", "Armor - support (dynamic)" );
        if( code == "FOLSPT" )
            return GetMissionId( missions, "INF Appuyer", "Infantry - support" );
        throw std::runtime_error( __FUNCTION__ ": Unsupported mission" );
    }

    // -----------------------------------------------------------------------------
    // Name: GetCodeFromMissionId
    // Created: SBO 2008-05-23
    // -----------------------------------------------------------------------------
    std::string GetCodeFromMissionId( const kernel::Resolver_ABC< kernel::MissionType >& missions, unsigned int id )
    {
        if( id == GetMissionId( missions, "ABC GTIA Freiner", "Battalion - Attack" ) )
            return "ATTACK";
        if( id == GetMissionId( missions, "ABC GTIA DonnerCoupArret", "Battalion - conduct a blocking action" ) )
            return "BLOCK";
        if( id == GetMissionId( missions, "ABC GTIA Freiner", "Battalion - delay" ) )
            return "DELAY";
        if( id == GetMissionId( missions, "ABC Reconnaitre Axe", "Armor - reconnoiter an axis" ) )
            return "RECCE";
        if( id == GetMissionId( missions, "INF Reconnaissance Axe", "Infantry - reconnoiter an axis" ) )
            return "RECCEF";
        if( id == GetMissionId( missions, "INF Semparer", "Infantry - seize" ) )
            return "SEIZNG";
        if( id == GetMissionId( missions, "INF Defendre Ferme", "Infantry - defend (until the end)" ) )
            return "HLDOFF";
        if( id == GetMissionId( missions, "Faire Mouvement", "move" ) )
            return "MOVE";
        if( id == GetMissionId( missions, "ABC Soutenir", "Armor - support (dynamic)" ) )
            return "FOLASS";
        if( id == GetMissionId( missions, "INF Appuyer", "Infantry - support" ) )
            return "FOLSPT";
        return "MOVE";
//        throw std::runtime_error( __FUNCTION__ ": Unsupported mission" );
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
        if( code == "AXIS" )
            return "path";
        if( code == "ENDRPT" )
            return "point";
        if( code == "automat" )
            return "automat";
        if( code == "agent" )
            return "agent";
        throw std::runtime_error( __FUNCTION__ ": Unsupported parameter type");
    }
}
