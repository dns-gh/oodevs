// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ExportFilter.h"
#include "preparation/Tools.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/operations.hpp>
#include <qsettings.h>
#include <qtextcodec.h>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace bfs = boost::filesystem;

namespace
{
    QString GetCurrentLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportFilter constructor
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
ExportFilter::ExportFilter( xml::xistream& xis, const tools::ExerciseConfig& config )
    : config_( config )
    , target_( xml::attribute< std::string >( xis, "target" ) )
    , xsl_( xml::attribute< std::string >( xis, "xsl" ) )
    , output_( xml::attribute< std::string >( xis, "output" ) )
    , currentLanguage_( GetCurrentLang().ascii() )
{
    try
    {
        xis >> xml::list( "description", *this, &ExportFilter::ReadDescription );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: ExportFilter destructor
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
ExportFilter::~ExportFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExportFilter::GetName
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
QString ExportFilter::GetName() const
{
    T_Descriptions::const_iterator it = descriptions_.find( currentLanguage_ );
    if( it != descriptions_.end() )
        return it->second.first;
    else
    {
        it = descriptions_.find( "en" );
        if( it != descriptions_.end() )
            return it->second.first;
    }
    return xsl_.c_str();
}

// -----------------------------------------------------------------------------
// Name: ExportFilter::GetDescription
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
QString ExportFilter::GetDescription() const
{
    T_Descriptions::const_iterator it = descriptions_.find( currentLanguage_ );
    if( it != descriptions_.end() )
        return it->second.second;
    else
    {
        it = descriptions_.find( "en" );
        if( it != descriptions_.end() )
            return it->second.second;
    }
    return tools::translate( "ExportFilter", "No description available" );
}

namespace
{
    std::string ResolveTargetFile( const std::string& target, const tools::ExerciseConfig& config )
    {
        if( target == "exercise" )
            return config.GetExerciseFile();
        if( target == "orbat" )
            return config.GetOrbatFile();
        return config.BuildExerciseChildFile( target );
    }

    std::string MakeOutputFile( const std::string& directory, const std::string& input, const std::string& type )
    {
        const std::string extension = type;
        bfs::path output( bfs::path( bfs::path( directory ) / bfs::path( input ).filename() ) );
        return output.replace_extension( extension ).native_file_string();
    }

    std::string MakeXsl( const std::string& file, const std::string& lang = "" )
    {
        const std::string extension( lang.empty() ? "_" + lang : "" );
        const std::string xsl( file + extension + ".xsl" );
        bfs::path filter( tools::GeneralConfig::BuildResourceChildFile( ( bfs::path( "export" ) / xsl ).native_file_string() ) );
        return filter.native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: ExportFilter::Execute
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportFilter::Execute( const std::string& outputDirectory ) const
{
    const std::string inputFile = ResolveTargetFile( target_, config_ );
    const std::string outputFile = MakeOutputFile( outputDirectory, inputFile, output_ );
    std::string xslFile( MakeXsl( xsl_, currentLanguage_ ) );
    if( ! bfs::exists( xslFile ) )
    {
        xslFile = MakeXsl( xsl_, "en" );
        if( ! bfs::exists( xslFile ) )
            xslFile = MakeXsl( xsl_ );
    }
    xsl::xftransform xft( xslFile, outputFile );
    xft << xml::xifstream( inputFile );
}

// -----------------------------------------------------------------------------
// Name: ExportFilter::ReadDescription
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void ExportFilter::ReadDescription( xml::xistream& xis )
{
    const std::string lang = xml::attribute< std::string >( xis, "xml:lang" );
    const std::string name = xml::attribute< std::string >( xis, "name" );
    std::string description;
    xis >> description;
    descriptions_[lang] = std::make_pair( name.c_str(), description.c_str() );
}
