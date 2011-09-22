// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterXsl.h"
#include "moc_FilterXsl.cpp"

#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace bfs = boost::filesystem;

namespace
{
    std::string ResolveInputFile( const std::string target, const tools::ExerciseConfig& config )
    {
        if( target == "orbat" )
            return config.GetOrbatFile();
        else if( target == "exercise" )
            return config.GetExerciseFile();
        else if( target == "profiles" )
            return config.GetProfilesFile();
        else if( target == "scores" )
            return config.GetScoresFile();
        else if( target == "success-factors" )
            return config.GetSuccessFactorsFile();
        else if( target == "urbanstate" )
            return config.GetUrbanStateFile();
        else if( target == "weather" )
            return config.GetWeatherFile();
        else
            return config.BuildExerciseChildFile( target );
    }
    std::string MakeOutputFile( const std::string& directory, const std::string& input, const std::string& type )
    {
        const std::string extension = type;
        bfs::path output( bfs::path( bfs::path( directory ) / bfs::path( input ).filename() ) );
        return output.replace_extension( extension ).native_file_string();
    }
    std::string MakeXsl( const std::string& file, const tools::ExerciseConfig& config, const std::string& lang = "" )
    {
        const std::string extension( lang.empty() ? "" : "_" + lang );
        const std::string xsl( file + extension + ".xsl" );
        bfs::path filter( config.BuildPhysicalChildFile( ( bfs::path( "Filters" ) / xsl ).native_file_string() ) );
        return filter.native_file_string();
    }
    std::string ResolveXslFile( const std::string& xsl, const std::string& lang, const tools::ExerciseConfig& config )
    {
        std::string xslFile( MakeXsl( xsl, config, lang ) );
        if( !bfs::exists( xslFile ) )
        {
            xslFile = MakeXsl( xsl, config, "en" );
            if( !bfs::exists( xslFile ) )
                xslFile = MakeXsl( xsl, config );
        }
        return xslFile;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterXsl constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterXsl::FilterXsl( xml::xistream& xis, const tools::ExerciseConfig& config )
    : Filter()
    , xsl_            ( xis.attribute< std::string >( "xsl" ) )
    , xslFile_        ( ResolveXslFile( xis.attribute< std::string >( "xsl" ), currentLanguage_, config ) )
    , inputFile_      ( ResolveInputFile( xis.attribute< std::string >( "target" ), config ) )
    , exerciseFile_   ( config.GetExerciseFile().c_str() )
    , outputExtension_( xis.attribute< std::string >( "output" ) )
    , output_         ( 0 )
{
    assert( !xsl_.empty() && !xslFile_.empty() && !inputFile_.empty() && !exerciseFile_.empty() && !outputExtension_.empty() );
    ReadDescriptions( xis );
}

// -----------------------------------------------------------------------------
// Name: FilterXsl destructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
FilterXsl::~FilterXsl()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::GetName
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string FilterXsl::GetName() const
{
    const std::string result = Filter::GetName();
    return ( result.empty() ) ? xsl_ : result;
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::OnBrowse
// Created: SBO 2010-04-22
// -----------------------------------------------------------------------------
void FilterXsl::OnBrowse()
{
    assert( !exerciseFile_.empty() );
    QString directory = Q3FileDialog::getExistingDirectory( exerciseFile_.c_str(), QApplication::activeModalWidget(), 0, tools::translate( "FilterXsl", "Select output directory" ) );
    if( directory.startsWith( "//" ) )
        directory.replace( "/", "\\" );
    output_->setText( directory );
    emit( statusChanged( IsValid() ) );
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::OnTextChanged
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void FilterXsl::OnTextChanged()
{
    emit( statusChanged( IsValid() ) );
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::IsValid
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
bool FilterXsl::IsValid() const
{
    return output_ && output_->text().ascii() && !output_->text().isEmpty() && bfs::exists( output_->text().ascii() );
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::CreateParametersWidget
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
QWidget* FilterXsl::CreateParametersWidget( QWidget* parent )
{
    Q3GroupBox* parametersWidget = new Q3GroupBox( 3, Qt::Horizontal, tools::translate( "FilterXsl", "Select output directory" ), parent, "FilterXsl_ParameterGroupBox" );
    new QLabel( tools::translate( "FilterXsl", "Output to:" ), parametersWidget, "FilterXsl_ParameterLabel" );
    output_ = new QLineEdit( tools::translate( "FilterXsl", "Enter output directory here" ), parametersWidget, "FilterXsl_ParameterLineEdit" );
    connect( output_, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged() ) );
    QPushButton* browseBtn = new QPushButton( tools::translate( "FilterXsl", "Browse..." ), parametersWidget, "FilterXsl_ParameterBrowseButton" );
    connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::Execute
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterXsl::Execute()
{
    assert( output_ && output_->text().ascii() && !output_->text().isEmpty() && !inputFile_.empty() );
    xsl::xftransform xft( xslFile_, MakeOutputFile( output_->text().ascii(), inputFile_, outputExtension_ ) );
    xft << xml::xifstream( inputFile_ );
}
