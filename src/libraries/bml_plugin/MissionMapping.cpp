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
        throw std::runtime_error( __FUNCTION__ ": Unsupported parameter type");
    }
}
