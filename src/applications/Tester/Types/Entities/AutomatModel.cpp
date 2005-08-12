// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_Pch.h"
#include "AutomatModel.h"
#include "Actions/Missions/Mission_Automat_Type.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: AutomatModel constructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
AutomatModel::AutomatModel( const std::string& strName, XmlInputArchive& archive )
    : strName_      ( strName )
    , missionTypes_ ()
{
    try   
    {
        std::string strValue;

        archive.BeginList( "Missions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Mission" );
            std::string strMissionName;
            archive.ReadAttribute( "nom", strMissionName );
            const Mission_Automat_Type* missionType = Mission_Automat_Type::Find( strMissionName );
            if( missionType )
                missionTypes_.push_back( missionType );
            else
                MT_LOG_WARNING_MSG( "Reading models: mission type \"" << strMissionName << "\" not found" );
            archive.EndSection(); // Mission
        }
        archive.EndList(); // Missions
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatModel destructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
AutomatModel::~AutomatModel()
{
    missionTypes_.clear();
}
