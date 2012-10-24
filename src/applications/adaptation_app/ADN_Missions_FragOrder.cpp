// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_FragOrder.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Missions_Data.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

ADN_Missions_FragOrder::ADN_Missions_FragOrder()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

ADN_Missions_FragOrder::ADN_Missions_FragOrder( unsigned int id )
    : id_                       ( id )
    , isAvailableWithoutMission_( false )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

ADN_Missions_FragOrder::~ADN_Missions_FragOrder()
{
    // NOTHING
}

std::string ADN_Missions_FragOrder::GetItemName()
{
    return strName_.GetData();
}

ADN_Missions_FragOrder* ADN_Missions_FragOrder::CreateCopy()
{
    ADN_Missions_FragOrder* newFragOrder = new ADN_Missions_FragOrder();
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->missionSheetContent_ = missionSheetContent_.GetData();
    newFragOrder->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newFragOrder->parameters_.AddItem( newParam );
    }
    return newFragOrder;
}

void ADN_Missions_FragOrder::ReadArchive( xml::xistream& input, const std::string& baseDir, const std::string& missionDir )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ )
          >> xml::optional >> xml::attribute( "available-without-mission", isAvailableWithoutMission_ )
          >> xml::list( "parameter", *this, &ADN_Missions_FragOrder::ReadParameter );
    ReadMissionSheet( baseDir, missionDir );
}

void ADN_Missions_FragOrder::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

namespace
{
    QString BuildDiaFragOrderType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
            list[0].append( "_" );
        for( int i = 1; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && list[i] == list[i].upper() )
                list[i].append( "_" );
        return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
    }
}

void ADN_Missions_FragOrder::WriteArchive( xml::xostream& output )
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaFragOrderType( strName_.GetData().c_str() ).toAscii().constData();

    output << xml::start( "fragorder" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "dia-type", diaType_ )
            << xml::attribute( "id", id_ )
            << xml::attribute( "available-without-mission", isAvailableWithoutMission_ );

    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end;
}

void ADN_Missions_FragOrder::ReadMissionSheet( const std::string& baseDir, const std::string& missionDir )
{
    missionSheetPath_ = std::string( missionDir +  "/" + strName_.GetData() + ".html" );
    const std::string dir = baseDir + missionDir;
    const std::string fileName = baseDir + missionSheetPath_;
    if( bfs::is_directory( dir ) && bfs::is_regular_file( fileName ) )
    {
        std::ifstream file( fileName.c_str() );
        std::stringstream buffer;
        buffer << file.rdbuf();
        missionSheetContent_ = std::string( buffer.str() );
        file.close();
    }
}

void ADN_Missions_FragOrder::RemoveDifferentNamedMissionSheet( const std::string& baseDir, const std::string& missionDir )
{
    const std::string relPath = std::string( missionDir + "/" + strName_.GetData() + ".html" );
    if( !missionSheetPath_.empty() && relPath != missionSheetPath_ )
        bfs::remove( baseDir + relPath );
}

void ADN_Missions_FragOrder::WriteMissionSheet( const std::string& baseDir, const std::string& missionDir )
{
    const std::string dir = baseDir + missionDir;
    std::string fileName = std::string(dir + "/" + strName_.GetData() + ".html" );

    if( !bfs::is_directory( dir ) )
        bfs::create_directories( dir + "/obsolete" );
    std::fstream fileStream( fileName.c_str(), std::ios::out | std::ios::trunc );
    fileStream << missionSheetContent_.GetData();
    fileStream.close();
    missionSheetPath_ = fileName;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_FragOrder::IsValidDatabase
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
bool ADN_Missions_FragOrder::IsValidDatabase()
{
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( !(*it)->IsValidDatabase() )
            return false;
    return true;
}

