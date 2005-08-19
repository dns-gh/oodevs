// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 15:07 $
// $Revision: 8 $
// $Workfile: PositionManager.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"
#include "Tools/PositionManager.h"
#include "Tools/Position.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"
#include "geometry/Polygon2.h"

using namespace TIC;

PositionManager::T_PolygonSet    PositionManager::areas_;

//-----------------------------------------------------------------------------
// Name: PositionManager::PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::PositionManager()
{
}

//-----------------------------------------------------------------------------
// Name: PositionManager::~PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::~PositionManager()
{
}

//-----------------------------------------------------------------------------
// Name: PositionManager::Initialize
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void PositionManager::Initialize( const std::string& strWorldConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strWorldConfigFile, strDir  );
        MT_ExtractFileName  ( strWorldConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive     archive;

        std::string         strTmp;
        archive.Open        ( strFile );
        archive.ReadField   ( "World", strTmp );
        archive.Close       ();

        Position::Initialize( strTmp );

        MT_ChangeDir        ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        throw exception;
    }
}

//-----------------------------------------------------------------------------
// Name: PositionManager::Terminate
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void PositionManager::Terminate()
{
    for( CIT_PolygonSet it = areas_.begin(); it != areas_.end(); ++it )
        delete *it;
    areas_.clear();

    Position::Terminate();
}

////-----------------------------------------------------------------------------
//// Name: PositionManager::AddArea
//// Created: SBO 2005-05-23
////-----------------------------------------------------------------------------
//void PositionManager::AddArea( const std::set< std::string >& mgrsPointSet )
//{
//    geometry::Polygon2< double >* poly = new geometry::Polygon2< double >();
//    for( std::set< std::string >::const_iterator it = mgrsPointSet.begin(); it != mgrsPointSet.end(); ++it )
//        poly->Add( Position( *it ) );
//    bool bOut = areas_.insert( poly ).second;
//    assert( bOut );
//}

//-----------------------------------------------------------------------------
// Name: PositionManager::AddArea
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
void PositionManager::AddArea( const T_DisaggregationArea& area )
{
    geometry::Polygon2< double >* poly = new geometry::Polygon2< double >();
    for( CIT_DisaggregationArea it = area.begin(); it != area.end(); ++it )
        poly->Add( Position( it->first, it->second ) );
    bool bOut = areas_.insert( poly ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: PositionManager::IsInAnArea
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
bool PositionManager::IsInAnArea( const Position& position )
{
    for( CIT_PolygonSet it = areas_.begin(); it != areas_.end(); ++it )
        if( ( *it )->IsInside( position ) )
            return true;
    return false;
}