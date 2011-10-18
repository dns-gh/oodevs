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
#include "clients_kernel/Tools.h"
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: FilterCsv constructor
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
FilterCsv::FilterCsv( const tools::ExerciseConfig& config, Model& model, const kernel::CoordinateConverter_ABC& converter )
    : pExport_     ( new CsvExport( model, converter ) )
    , output_      ( 0 )
    , exerciseFile_( config.GetExerciseFile().c_str() )
{
        // NOTHING
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
    bfs::path ouput( output_->text().ascii() );
    pExport_->Execute( ouput );
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
    return tools::translate( "FilterCsv", "Export orbat, resources, stocks, weather schedule, diplomacy matrix, profiles and logistic links as CSV files." ).toStdString();;
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::CreateParametersWidget
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
QWidget* FilterCsv::CreateParametersWidget( QWidget* parent )
{
    Q3GroupBox* parametersWidget = new Q3GroupBox( 3, Qt::Horizontal, tools::translate( "FilterCsv", "Select output directory" ), parent );
    new QLabel( tools::translate( "FilterCsv", "Output to:" ), parametersWidget );
    output_ = new QLineEdit( tools::translate( "FilterCsv", "Enter output directory here" ), parametersWidget );
    connect( output_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged() ) );
    QPushButton* browseBtn = new QPushButton( tools::translate( "FilterCsv", "Browse..." ), parametersWidget );
    connect( browseBtn, SIGNAL( clicked() ), this, SLOT( OnBrowse() ) );
    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterCsv::IsValid
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
bool FilterCsv::IsValid() const
{
    return output_ && output_->text().ascii() && !output_->text().isEmpty() && bfs::exists( output_->text().ascii() );
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
    QString directory = Q3FileDialog::getExistingDirectory( exerciseFile_.c_str(), QApplication::activeModalWidget(),
                                                            0, tools::translate( "FilterCsv", "Select output directory" ) );
    if( directory.startsWith( "//" ) )
        directory.replace( "/", "\\" );
    output_->setText( directory );
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
