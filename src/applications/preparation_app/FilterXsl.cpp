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

#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichPushButton.h"
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

namespace
{
    tools::Path ResolveInputFile( const tools::Path& target, const tools::ExerciseConfig& config )
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
    tools::Path MakeOutputFile( const tools::Path& directory, const tools::Path& input, const tools::Path& type )
    {
        tools::Path result = directory / input.FileName();
        return result.ReplaceExtension( type );
    }
    tools::Path MakeXsl( const tools::Path& file, const tools::ExerciseConfig& config, const std::string& lang = "" )
    {
        const tools::Path extension = tools::Path::FromUTF8( lang.empty() ? "" : "_" + lang );
        const tools::Path xsl = file + extension + ".xsl";
        return config.BuildPhysicalChildFile( tools::Path( "Filters" ) / xsl );
    }
    tools::Path ResolveXslFile( const tools::Path& xsl, const std::string& lang, const tools::ExerciseConfig& config )
    {
        tools::Path xslFile = MakeXsl( xsl, config, lang );
        if( !xslFile.Exists() )
        {
            xslFile = MakeXsl( xsl, config, "en" );
            if( !xslFile.Exists() )
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
    : Filter( xis )
    , xsl_            ( xis.attribute< std::string >( "xsl" ) )
    , xslFile_        ( ResolveXslFile( xis.attribute< tools::Path >( "xsl" ), description_.GetCurrentLanguage(), config ) )
    , inputFile_      ( ResolveInputFile( xis.attribute< tools::Path >( "target" ), config ) )
    , exerciseFile_   ( config.GetExerciseFile() )
    , outputExtension_( xis.attribute< tools::Path >( "output" ) )
    , output_         ( 0 )
{
    assert( !xsl_.empty() && !xslFile_.IsEmpty() && !inputFile_.IsEmpty() && !exerciseFile_.IsEmpty() && !outputExtension_.IsEmpty() );
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
    assert( !exerciseFile_.IsEmpty() );
    tools::Path directory = gui::FileDialog::getExistingDirectory( QApplication::activeModalWidget(), tools::translate( "FilterXsl", "Select output directory" ), exerciseFile_ );
    output_->setText( directory.ToUTF8().c_str() );
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
    return output_ && !output_->text().isEmpty() && tools::Path::FromUnicode( output_->text().toStdWString() ).Exists();
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::CreateParametersWidget
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
QWidget* FilterXsl::CreateParametersWidget( const QString& objectName, QWidget* parent )
{
    gui::SubObjectName subObject( objectName );

    QLabel* outputLabel  = new QLabel( tools::translate( "FilterXsl", "Output to:" ) );

    output_ = new gui::RichLineEdit( "output", tools::translate( "FilterXsl", "Enter output directory here" ) );
    connect( output_, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged() ) );

    gui::RichPushButton* browseBtn = new gui::RichPushButton( "browse", tools::translate( "FilterXsl", "Browse..." ) );
    connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );

    gui::RichGroupBox* parametersWidget = new gui::RichGroupBox( "ParameterGroupBox", tools::translate( "FilterXsl", "Select output directory" ), parent );
    QHBoxLayout* parametersWidgetLayout = new QHBoxLayout( parametersWidget );
    parametersWidgetLayout->addWidget( outputLabel );
    parametersWidgetLayout->addWidget( output_ );
    parametersWidgetLayout->addWidget( browseBtn );

    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterXsl::Execute
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void FilterXsl::Execute()
{
    assert( output_ && !output_->text().isEmpty() && !inputFile_.IsEmpty() );
    xsl::xftransform xft( xslFile_.ToUTF8().c_str(), MakeOutputFile( tools::Path::FromUnicode( output_->text().toStdWString() ), inputFile_, outputExtension_ ).ToUTF8().c_str() );
    xft << tools::Xifstream( inputFile_ );
}
