// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Mission.h"

#include "ADN_Missions_Data.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::ADN_Missions_Mission( unsigned int id )
    : id_( id )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::~Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission::~ADN_Missions_Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Mission::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Mission* ADN_Missions_Mission::CreateCopy()
{
    ADN_Missions_Mission* newMission     = new ADN_Missions_Mission();
    newMission->strName_                 = strName_.GetData();
    newMission->diaBehavior_             = diaBehavior_.GetData();
    newMission->cdtDiaBehavior_          = cdtDiaBehavior_.GetData();
    newMission->mrtDiaBehavior_          = mrtDiaBehavior_.GetData();
    newMission->missionSheetContent_     = missionSheetContent_.GetData();
    newMission->symbol_                  = symbol_.GetData();
    newMission->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ADN_Missions_Parameter* newParam = (*it)->CreateCopy();
        newMission->parameters_.AddItem( newParam );
    }
    return newMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadArchive( xml::xistream& input, std::size_t contextLength,
    ADN_Drawings_Data& drawings, const std::string& baseDir, const std::string& missionDir )
{
    std::string missionSheetDesc, symbol;
    std::size_t index = 0;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "dia-type", diaType_ )
        >> xml::optional >> xml::attribute( "symbol", symbol )
        >> xml::optional >> xml::attribute( "dia-behavior", diaBehavior_ )
        >> xml::optional >> xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "package", strPackage_ )
        >> xml::list( "parameter", boost::bind( &ADN_Missions_Mission::ReadParameter, this , _1,  boost::ref( index ), contextLength ) );
    const std::string code = symbol.empty() ? " - " : symbol;
    symbol_.SetVector( drawings.GetCategoryDrawings( "tasks" ) );
    symbol_.SetData( drawings.GetDrawing( code ) );
    ReadMissionSheet( baseDir, missionDir );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength )
{
    index++;
    if( index > contextLength )
    {
        std::auto_ptr< ADN_Missions_Parameter > spNew( new ADN_Missions_Parameter() );
        spNew->ReadArchive( input );
        parameters_.AddItem( spNew.release() );
    }
}

namespace
{
    QString BuildDiaMissionType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( int i = 0; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && ( list[i] == list[i].upper() || list[i].lower() == "test" ) )
                list[i].append( "_" );
        return list.join( "" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Mission::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteArchive( xml::xostream& output, const std::string& type, const T_MissionParameter_Vector& context )
{
    output << xml::start( "mission" );
    bool isAutomat = type == "automats";
    const QString typeName = type == "units" ? "Pion" : (isAutomat ? "Automate" : "Population");
    const QString diaName  = BuildDiaMissionType( strName_.GetData().c_str() );
    if( diaType_.GetData().empty() )
        diaType_ = QString( "T_Mission_%1_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();

    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );

    const std::string code = ( symbol_.GetData() ) ? symbol_.GetData()->GetCode() : "";
    if( code != "" && code != " - " )
        output << xml::attribute( "symbol", code );

    if( ! strPackage_.GetData().empty() )
        output << xml::attribute( "package", strPackage_);

    if( !isAutomat )
    {
        if( diaBehavior_.GetData().empty() )
            diaBehavior_ = QString( "MIS_%1_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        output << xml::attribute( "dia-behavior", diaBehavior_ );
    }
    else
    {
        if( cdtDiaBehavior_.GetData().empty() )
            cdtDiaBehavior_ = QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        if( mrtDiaBehavior_.GetData().empty() )
            mrtDiaBehavior_ = QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        output << xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
               << xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ );
    }
    if( ! context.empty() )
    {
        for( unsigned i = 0; i < context.size(); ++i )
            context[i]->WriteArchive( output );
    }
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::ReadMissionSheet( const std::string& baseDir, const std::string& missionDir )
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

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::RemoveMissionSheet
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::RemoveDifferentNamedMissionSheet( const std::string& baseDir, const std::string& missionDir )
{
    const std::string relPath = std::string( missionDir + "/" + strName_.GetData() + ".html" );
    if( !missionSheetPath_.empty() && relPath != missionSheetPath_ )
        bfs::remove( baseDir + relPath );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteMissionSheet
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_Missions_Mission::WriteMissionSheet( const std::string& baseDir, const std::string& missionDir )
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
// Name: ADN_Missions_Mission::IsValidDatabase
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
bool ADN_Missions_Mission::IsValidDatabase()
{
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
        if( !(*it)->IsValidDatabase() )
            return false;
    return true;
}

