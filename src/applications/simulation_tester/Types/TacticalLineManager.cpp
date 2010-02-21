// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TacticalLineManager.h"
#include "TacticalLines/TacticalLine_ABC.h"
#include "TacticalLines/TacticalLine_Limit.h"
#include "TacticalLines/TacticalLine_Lima.h"
#include "Tools/PositionManager.h"
#include "Tools/Position.h"

#include "MT/MT_IO/MT_DirectoryBrowser.h"
#include "MT/MT_IO/MT_Dir.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TacticalLineManager constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLineManager::TacticalLineManager( const PositionManager& /*posMgr*/ )
    : lines_ ()
{
    //CreateDefaultTacticalLines( posMgr );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::LoadTacticalLines
// Created: SBO 2005-08-23
// -----------------------------------------------------------------------------
void TacticalLineManager::LoadTacticalLines( const std::string& strConfigFile )
{
    std::string           strCurrentDir = MT_GetCurrentDir();
    std::string           strDir;
    std::string           strFile;

    MT_ExtractFilePath    ( strConfigFile, strDir  );
    MT_ExtractFileName    ( strConfigFile, strFile );

    try
    {
        XmlInputArchive   archive;

        MT_ChangeDir      ( strDir );
        archive.Open      ( strFile );

        archive.BeginList( "lines" );
        while( archive.NextListElement() )
        { 
            if( archive.BeginList( "limit", XmlInputArchive::eNothing ) )
            {
                Register( *new TacticalLine_Limit( archive ) );
                archive.EndList(); // Limit
            }
            else if( archive.BeginList( "lima", XmlInputArchive::eNothing ) )
            {
                Register( *new TacticalLine_Lima( archive ) );
                archive.EndList(); // Lima
            }
        }
        archive.EndList(); // Lines
            
        archive.Close     ();

        MT_ChangeDir      ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_ChangeDir    ( strCurrentDir );
        MT_LOG_ERROR_MSG( exception.what() << "Parse error" );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLineManager::~TacticalLineManager()
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        delete *it;
    lines_.clear();
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::CreateDefaultTacticalLines( const PositionManager& posMgr )
{
    // Create default limits: world borders
    Position topLeft;
    Position topRight;
    Position bottomRight;
    Position bottomLeft;
    topLeft    .SetSimCoordinates(                          0, posMgr.GetWorldHeight() - 1 );
    topRight   .SetSimCoordinates( posMgr.GetWorldWidth() - 1, posMgr.GetWorldHeight() - 1 );
    bottomRight.SetSimCoordinates( posMgr.GetWorldWidth() - 1,                           0 );
    bottomLeft .SetSimCoordinates(                          0,                           0 );

    T_PositionVector pointsLeft;
    pointsLeft.push_back( &bottomLeft );
    pointsLeft.push_back( &topLeft    );
    Register( *new TacticalLine_Limit( pointsLeft ) );

    T_PositionVector pointsRight;
    pointsRight.push_back( &bottomRight );
    pointsRight.push_back( &topRight    );
    Register( *new TacticalLine_Limit( pointsRight ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Register
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::Register( TacticalLine_ABC& line )
{
    lines_.insert( &line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::UnRegister
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::UnRegister( TacticalLine_ABC& line )
{
    lines_.erase( &line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Find
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_ABC* TacticalLineManager::Find( T_EntityId nId  )
{
    for( IT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        if( ( *it )->GetId() == nId )
            return *it;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: TacticalLineManager::GetNextLimitId
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
T_EntityId TacticalLineManager::GetNextLimitId()
{
    static unsigned int nStart = 0;

    unsigned int nCur = 0;
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it, ++nCur )
        if( nCur >= nStart && ( *it )->GetLineType() == TacticalLine_ABC::eLimit )
        {
            nStart = ( nCur + 1 ) % lines_.size();
            return ( *it )->GetId();
        }
    // if we went to end of list, try to find a limit from the beginning
    nCur   = 0;
    nStart = 0;
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it, ++nCur )
        if( nCur >= nStart && ( *it )->GetLineType() == TacticalLine_ABC::eLimit )
        {
            nStart = ( nCur + 1 ) % lines_.size();
            return ( *it )->GetId();
        }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::GetLimaByType
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
T_EntityId TacticalLineManager::GetLimaByType( EnumLimaType eLimaType )
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        if( ( *it )->GetLineType() == TacticalLine_ABC::eLima && 
            static_cast< const TacticalLine_Lima* >( *it )->GetLimaType() == eLimaType )
            return ( *it )->GetId();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::UpdateToSim
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::UpdateToSim()
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        ( *it )->UpdateToSim();
}
