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

#include "FilterPartiesListView.h"
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "tools/ExerciseConfig.h"
#include "tools/IdManager.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichGroupBox.h"
#include <xeumeuleu/xml.h>

namespace
{
    bool IsIndex( const std::string& attribute )
    {
        return attribute == "id" || attribute == "knowledge-group" || attribute == "party";
    }

    gui::RichCheckBox* AddCheckBox( QWidget* parent, const QString& text, const QString& name, bool checked = true )
    {
        gui::RichCheckBox* checkbox = new gui::RichCheckBox( name, text );
        checkbox->setChecked( checked );
        parent->layout()->addWidget( checkbox );
        parent->layout()->addWidget( checkbox );
        return checkbox;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
FilterOrbatReIndexer::FilterOrbatReIndexer( QWidget* mainwindow, const tools::ExerciseConfig& config, Model& model )
    : Filter()
    , model_                ( model )
    , orbatFile_            ( config.GetOrbatFile() )
    , isUrbanBlock_         ( false )
    , shift_                ( 0 )
    , objectsCheckBox_      ( 0 )
    , crowdsCheckBox_       ( 0 )
    , populationsCheckBox_  ( 0 )
    , initialStateCheckBox_ ( 0 )
    , logisticLinksCheckBox_( 0 )
    , stocksCheckBox_       ( 0 )
    , diplomacyCheckBox_    ( 0 )
{
    connect( this, SIGNAL( DoConsistencyCheck() ), mainwindow, SIGNAL( CheckConsistency() ) );
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
    return tools::translate( "FilterOrbatReIndexer", "Import an order of battle" ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::GetDescription
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
const std::string FilterOrbatReIndexer::GetDescription() const
{
    return tools::translate( "FilterOrbatReIndexer", "Import an order of battle into current exercise.<br>May import the populations and the objects if relative checkbox are checked." ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::IsValid
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
bool FilterOrbatReIndexer::IsValid() const
{
    return filename_ && !filename_->text().isEmpty() && tools::Path::FromUnicode( filename_->text().toStdWString() ).Exists();
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::OnBrowse
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::OnBrowse()
{
    assert( !orbatFile_.IsEmpty() );
    tools::Path filename = gui::FileDialog::getOpenFileName( QApplication::activeModalWidget(), tools::translate( "FilterOrbatReIndexer", "Load orbat file" ), orbatFile_, "Orbat (*.xml)" );
    filename_->setText( filename.ToUTF8().c_str() );
    emit( statusChanged( IsValid() && partiesListView_->ParseOrbatFile( tools::Path::FromUnicode( filename_->text().toStdWString() ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadField
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadField( const std::string& name, xml::xistream& xis, xml::xostream& xos )
{
    if( name == "object" && !objectsCheckBox_->isChecked() ||
        name == "population" && !crowdsCheckBox_->isChecked() ||
        ( name == "equipments" || name == "humans" || name == "resources" ) && !initialStateCheckBox_->isChecked() ||
        name == "stocks" && !stocksCheckBox_->isChecked() )
        return;
    if( name == "party" )
    {
        unsigned long partyID = xis.attribute< unsigned long >( "id", 0 );
        if( partyID != 0 && !partiesListView_->IsPartyChecked( partyID ) )
            return;
    }

    xos << xml::start( name );
    if( !( name == "logistics" && !logisticLinksCheckBox_->isChecked() ||
           name == "diplomacy" && !diplomacyCheckBox_->isChecked() ||
           name == "inhabitants" && !populationsCheckBox_->isChecked() ) )
    {
        isUrbanBlock_ = name == "urban-block";
        if( name == "diplomacy" )
            xis >> xml::list( "party", *this, &FilterOrbatReIndexer::ReadDiplomacy, xos );
        else
        {
            std::string text;
            xis >> xml::attributes( *this, &FilterOrbatReIndexer::ReadAttribute, xos )
                >> xml::list( *this, &FilterOrbatReIndexer::ReadField, xos )
                >> xml::optional >> text;
            if( !text.empty() ) // $$$$ SBO 2008-04-08: check for cdata
                xos << text;
        }
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadAttribute
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadAttribute( const std::string& name, xml::xistream& xis, xml::xostream& xos )
{
    if( !isUrbanBlock_ && IsIndex( name ) )
        xos << xml::attribute( name, xis.value< unsigned long >() + shift_ );
    else
        xos << xml::attribute( name, xis.value< std::string >() );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadPartyDiplomacy
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadDiplomacy( xml::xistream& xis, xml::xostream& xos )
{
    unsigned long partyID = xis.attribute< unsigned long >( "id", 0 );
    if( partyID != 0 && partiesListView_->IsPartyChecked( partyID ) )
    {
        xos << xml::start( "party" );
        xis >> xml::attributes( *this, &FilterOrbatReIndexer::ReadAttribute, xos )
            >> xml::list( "relationship", *this, &FilterOrbatReIndexer::ReadDiplomacyLink, xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadDiplomacyLink
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadDiplomacyLink( xml::xistream& xis, xml::xostream& xos )
{
    unsigned long partyID = xis.attribute< unsigned long >( "party", 0 );
    if( partyID != 0 && partiesListView_->IsPartyChecked( partyID ) )
    {
        xos << xml::start( "relationship" );
        xis >> xml::attributes( *this, &FilterOrbatReIndexer::ReadAttribute, xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::CreateParametersWidget
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
QWidget* FilterOrbatReIndexer::CreateParametersWidget( const QString& objectName, QWidget* parent )
{
    gui::SubObjectName subObject( objectName );
    gui::RichGroupBox* parametersWidget = new gui::RichGroupBox( "ParameterGroupBox", tools::translate( "FilterOrbatReIndexer", "Import parameters" ), parent );
    parametersWidget->setLayout( new QHBoxLayout() );
    {
        Q3HBox* hbox = new Q3HBox( parametersWidget, "FilterOrbatReIndexer_OrbatHBox" );
        hbox->setSpacing( 5 );
        new QLabel( tools::translate( "FilterOrbatReIndexer", "Select order of battle:" ), hbox, "FilterOrbatReIndexer_OrbatLabel" );
        filename_ = new gui::RichLineEdit( "FilterOrbatReIndexer", tools::translate( "FilterOrbatReIndexer", "Enter path to the new orbat here" ), hbox );
        filename_->setReadOnly( true );
        gui::RichPushButton* browseBtn = new gui::RichPushButton( "browseBtn", tools::translate( "FilterOrbatReIndexer", "Browse..." ), hbox );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    }
    {
        Q3HBox* hbox = new Q3HBox( parametersWidget, "ParameterHBox" );
        gui::RichGroupBox* optionBox = new gui::RichGroupBox( "OptionGroupBox", tools::translate( "FilterOrbatReIndexer", "Import options:" ), hbox );
        optionBox->setLayout( new QHBoxLayout() );
        objectsCheckBox_       = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Objects" )       , "ObjectsCheckBox" );
        crowdsCheckBox_        = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Crowds" )        , "CrowdsCheckBox");
        populationsCheckBox_   = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Populations" )   , "PopulationsCheckBox");
        initialStateCheckBox_  = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Initial state" ) , "InitialStateCheckBox");
        logisticLinksCheckBox_ = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Logistic links" ), "LogisticLinksCheckBox");
        stocksCheckBox_        = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Stocks" )        , "StocksCheckBox");
        diplomacyCheckBox_     = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Diplomacy" )     , "DiplomacyCheckBox");

        gui::RichGroupBox* sideBox = new gui::RichGroupBox( "PartiesGroupBox", tools::translate( "FilterOrbatReIndexer", "Parties:" ), hbox );
        partiesListView_ = new FilterPartiesListView( sideBox );
        partiesListView_->setEnabled( false );
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
    tools::Xifstream xis( tools::Path::FromUnicode( filename_->text().toStdWString() ) );
    xml::xostringstream xos;
    xos << xml::start( "orbat" );
    xis >> xml::start( "orbat" )
            >> xml::list( *this, &FilterOrbatReIndexer::ReadField, xos )
        >> xml::end;
    xos << xml::end;
    xml::xistringstream newXis( xos.str() );
    // Reload teams model
    model_.teams_.Load( newXis, model_ );
    emit( DoConsistencyCheck() );
}
