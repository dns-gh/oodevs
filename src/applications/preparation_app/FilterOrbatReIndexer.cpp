// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterOrbatReIndexer.h"
#include "moc_FilterOrbatReIndexer.cpp"

#include "clients_kernel/Tools.h"
#include "preparation/IdManager.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.h>

namespace
{
    bool IsIndex( const std::string& attribute )
    {
        return attribute == "id" || attribute == "knowledge-group" || attribute == "party";
    }
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
FilterOrbatReIndexer::FilterOrbatReIndexer( const tools::ExerciseConfig& config, Model& model )
    : Filter()
    , model_    ( model )
    , orbatFile_( config.GetOrbatFile() )
    , shift_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer destructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
FilterOrbatReIndexer::~FilterOrbatReIndexer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::GetName
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
const std::string FilterOrbatReIndexer::GetName() const
{
    return tools::translate( "FilterOrbatReIndexer", "Import an order of battle" ).ascii();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::GetDescription
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
const std::string FilterOrbatReIndexer::GetDescription() const
{
    return tools::translate( "FilterOrbatReIndexer", "Import an order of battle into current exercise.<br>May import the populations and the objects if relative checkbox are checked." ).ascii();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::IsValid
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
bool FilterOrbatReIndexer::IsValid() const
{
    return filename_ && filename_->text() && !filename_->text().isEmpty() && boost::filesystem::exists( filename_->text().ascii() );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::OnBrowse
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::OnBrowse()
{
    assert( !orbatFile_.empty() );
    QString filename = QFileDialog::getOpenFileName( orbatFile_.c_str(), "Orbat (*.xml)", QApplication::activeModalWidget(), 0, tools::translate( "FilterOrbatReIndexer", "Load orbat file" ) );
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    filename_->setText( filename );
    LoadPreview();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::LoadPreview
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::LoadPreview()
{
    preview_->clear();
    bool status = IsValid();
    if( status )
        try
        {
            xml::xifstream xis( filename_->text().ascii() );
            xis >> xml::start( "orbat" )
                    >> xml::start( "parties" )
                        >> xml::list( "party", *this, &FilterOrbatReIndexer::ReadTeam );
        }
        catch( ... )
        {
            QMessageBox::critical( QApplication::activeModalWidget(), tools::translate( "FilterOrbatReIndexer", "Error loading file" ), tools::translate( "FilterOrbatReIndexer", "File does not appear to be a valid orbat file.<br>Please select an other orbat file to import." ) );
            status = false;
        }
    emit( statusChanged( status ) );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadTeam
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadTeam( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    QListViewItem* item = new QListViewItem( preview_ );
    item->setText( 0, name.c_str() );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadField
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadField( const std::string& name, xml::xistream& xis, xml::xostream& xos )
{
    if( ( name == "object" && !objectsCheckBox_->isChecked() ) ||
        ( name == "population" && !populationsCheckBox_->isChecked() ) )
        return;
    xos << xml::start( name );
    std::string text;
    xis >> xml::attributes( *this, &FilterOrbatReIndexer::ReadAttribute, xos )
        >> xml::list( *this, &FilterOrbatReIndexer::ReadField, xos )
        >> xml::optional >> text;
    if( !text.empty() ) // $$$$ SBO 2008-04-08: check for cdata
        xos << text;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadAttribute
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadAttribute( const std::string& name, xml::xistream& xis, xml::xostream& xos )
{
    if( IsIndex( name ) )
        xos << xml::attribute( name, xis.value< unsigned long >() + shift_ );
    else
        xos << xml::attribute( name, xis.value< std::string >() );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::CreateParametersWidget
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
QWidget* FilterOrbatReIndexer::CreateParametersWidget( QWidget* parent )
{
    QGroupBox* parametersWidget = new QGroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Import parameters" ), parent, "FilterOrbatReIndexer_ParameterGroupBox" );
    {
        QHBox* hbox = new QHBox( parametersWidget, "FilterOrbatReIndexer_OrbatHBox" );
        hbox->setSpacing( 5 );
        new QLabel( tools::translate( "FilterOrbatReIndexer", "Select order of battle:" ), hbox, "FilterOrbatReIndexer_OrbatLabel" );
        filename_ = new QLineEdit( tools::translate( "FilterOrbatReIndexer", "Enter path to the new orbat here" ), hbox, "FilterOrbatReIndexer_OrbatLineEdit" );
        filename_->setReadOnly( true );
        QPushButton* browseBtn = new QPushButton( tools::translate( "FilterOrbatReIndexer", "Browse..." ), hbox, "FilterOrbatReIndexer_OrbatBrowseButton" );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    }
    {
        QHBox* hbox = new QHBox( parametersWidget, "FilterOrbatReIndexer_ParameterHBox" );
        QGroupBox* optionBox = new QGroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Import options:" ), hbox, "FilterOrbatReIndexer_PartiesGroupBox" );
        objectsCheckBox_ = new QCheckBox( tools::translate( "FilterOrbatReIndexer", "Objects" ), optionBox, "FilterOrbatReIndexer_ObjectsCheckBox" );
        objectsCheckBox_->setChecked( true );
        populationsCheckBox_ = new QCheckBox( tools::translate( "FilterOrbatReIndexer", "Crowd" ), optionBox, "FilterOrbatReIndexer_PopulationsCheckBox" );
        populationsCheckBox_->setChecked( true );
        QGroupBox* sideBox = new QGroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Parties overview:" ), hbox, "FilterOrbatReIndexer_PartiesGroupBox" );
        preview_ = new QListView( sideBox, "FilterOrbatReIndexer_PartiesListView" );
        preview_->setMaximumHeight( 50 );
        preview_->addColumn( "" );
        preview_->setResizeMode( QListView::LastColumn );
        preview_->header()->hide();
    }
    return parametersWidget;
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::Execute
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::Execute()
{
    // Create new order of battle
    shift_ = model_.GetIdManager().GetNextId();
    xml::xifstream xis( filename_->text().ascii() );
    xml::xostringstream xos;
    xos << xml::start( "orbat" );
    xis >> xml::start( "orbat" )
            >> xml::list( *this, &FilterOrbatReIndexer::ReadField, xos )
        >> xml::end;
    xos << xml::end;
    xml::xistringstream newXis( xos.str() );
    // Reload teams model
    std::string loadingErrors;
    model_.teams_.Load( newXis, model_, loadingErrors );
    if( !loadingErrors.empty() )
        QMessageBox::critical( QApplication::activeModalWidget(), tools::translate( "FilterOrbatReIndexer", "Error loading the new orbat file" ), ( tools::translate( "FilterOrbatReIndexer", "The following entities cannot be loaded: " ) + "\n" + loadingErrors.c_str() ).ascii() );
}
