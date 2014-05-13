// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterCsv.h"
#include "moc_FilterCsv.cpp"
#include "CsvExport.h"
#include "tools/ExerciseConfig.h"
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"

// -----------------------------------------------------------------------------
// Name: FilterCsv constructor
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
FilterCsv::FilterCsv( QWidget* parent, const tools::ExerciseConfig& config, Model& model,
                      const kernel::CoordinateConverter_ABC& converter )
    : progressDialog_( new QProgressDialog( parent, Qt::SplashScreen ) )
    , pExport_       ( new CsvExport( model, converter ) )
    , output_        ( 0 )
    , exerciseFile_  ( config.GetExerciseFile() )
    , exerciseName_  ( config.GetExerciseName() )
{
    progressDialog_->setAutoClose( true );
    progressDialog_->setCancelButton( 0 );
    progressDialog_->setMinimum( 0 );
    progressDialog_->setMaximum( 100 );
}

// -----------------------------------------------------------------------------
// Name: FilterCsv destructor
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
FilterCsv::~FilterCsv()
{
        // NOTHING
}

namespace
{
    std::string GetSeparator()
    {
        char buffer[256];
        GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SLIST, buffer, 256 );
        return buffer;
    }
}
// -----------------------------------------------------------------------------
// Name: FilterCsv::Execute
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void FilterCsv::Execute()
{
    pExport_->Execute( GetExerciseName(), tools::Path::FromUnicode( output_->text().toStdWString() ), *this );
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::Update
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void FilterCsv::Update( unsigned int value )
{
    if( value == 0 )
        progressDialog_->show();
    progressDialog_->setValue( value );
    qApp->processEvents();
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::GetName
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
const std::string FilterCsv::GetName() const
{
    return tools::translate( "FilterCsv", "CSV export" ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::GetDescription
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
const std::string FilterCsv::GetDescription() const
{
    return tools::translate( "FilterCsv", "Export orbat, supplies, stocks, weather schedule, diplomacy matrix, profiles and logistic links as CSV files." ).toStdString();;
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::GetExerciseName
// Created: NPT 2013-04-29
// -----------------------------------------------------------------------------
const tools::Path FilterCsv::GetExerciseName() const
{
    return exerciseName_.BaseName() + "_";
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::CreateParametersWidget
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
QWidget* FilterCsv::CreateParametersWidget( const QString& objectName,  QWidget* parent )
{
    gui::SubObjectName subObject( objectName );

    output_ = new gui::RichLineEdit( "output", tools::translate( "FilterCsv", "Enter output directory here" ) );
    connect( output_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged() ) );

    gui::RichPushButton* browseBtn = new gui::RichPushButton( "browseBtn", tools::translate( "FilterCsv", "Browse..." ) );
    connect( browseBtn, SIGNAL( clicked() ), this, SLOT( OnBrowse() ) );

    gui::RichGroupBox* parametersWidget = new gui::RichGroupBox( "parametersWidget", tools::translate( "FilterCsv", "Select output directory" ), parent );
    QHBoxLayout* parametersLayout = new QHBoxLayout( parametersWidget );
    parametersLayout->addWidget( new QLabel( tools::translate( "FilterCsv", "Output to:" ) ) );
    parametersLayout->addWidget( output_ );
    parametersLayout->addWidget( browseBtn );
    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::IsValid
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
bool FilterCsv::IsValid() const
{
    return output_ && !output_->text().isEmpty() && tools::Path::FromUnicode( output_->text().toStdWString() ).Exists();
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::NeedToReloadExercise
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
bool FilterCsv::NeedToReloadExercise() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::OnBrowse
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void FilterCsv::OnBrowse()
{
    tools::Path directory = gui::FileDialog::getExistingDirectory( QApplication::activeModalWidget(), tools::translate( "FilterCsv", "Select output directory" ), exerciseFile_ );
    output_->setText( directory.ToUTF8().c_str() );
    emit( statusChanged( IsValid() ) );
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::OnTextChanged:
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void FilterCsv::OnTextChanged()
{
    emit( statusChanged( IsValid() ) );
}
