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

#include "clients_gui/resources.h"
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

    QCheckBox* AddCheckBox( QWidget* parent, const QString& text, const QString& name, bool checked = true )
    {
        QCheckBox* checkbox = new QCheckBox( text, parent, name );
        checkbox->setChecked( checked );
        return checkbox;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
FilterOrbatReIndexer::FilterOrbatReIndexer( const tools::ExerciseConfig& config, Model& model )
    : Filter()
    , model_                ( model )
    , orbatFile_            ( config.GetOrbatFile() )
    , shift_                ( 0 )
    , objectsCheckBox_      ( 0 )
    , populationsCheckBox_  ( 0 )
    , initialStateCheckBox_ ( 0 )
    , logisticLinksCheckBox_( 0 )
    , stocksCheckBox_       ( 0 )
    , diplomacyCheckBox_    ( 0 )
    , checkedPixmap_        ( MAKE_PIXMAP( check ) )
    , uncheckedPixmap_      ( MAKE_PIXMAP( cross ) )
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
    return filename_ && filename_->text().ascii() && !filename_->text().isEmpty() && boost::filesystem::exists( filename_->text().ascii() );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::OnBrowse
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::OnBrowse()
{
    assert( !orbatFile_.empty() );
    QString filename = Q3FileDialog::getOpenFileName( orbatFile_.c_str(), "Orbat (*.xml)", QApplication::activeModalWidget(), 0, tools::translate( "FilterOrbatReIndexer", "Load orbat file" ) );
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
    partiesListView_->clear();
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
    partiesListView_->setEnabled( true );
    emit( statusChanged( status ) );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadTeam
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadTeam( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name", "" );
    unsigned long id = xis.attribute< unsigned long >( "id", 0 );
    assert( !name.empty() && id != 0 );
    Q3ListViewItem* item = new Q3ListViewItem( partiesListView_ );
    item->setText( ePartyName, name.c_str() );
    item->setText( eHiddenPartyID, QString::number( id ) );
    item->setText( eHiddenCheckbox, QString::number( 1 ) );
    item->setPixmap( eCheckbox, checkedPixmap_ );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::ReadField
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadField( const std::string& name, xml::xistream& xis, xml::xostream& xos )
{
    if( name == "object" && !objectsCheckBox_->isChecked() ||
        name == "population" && !populationsCheckBox_->isChecked() ||
        ( name == "equipments" || name == "humans" || name == "resources" ) && !initialStateCheckBox_->isChecked() ||
        name == "stocks" && !stocksCheckBox_->isChecked() )
        return;
    if( name == "logistics" && !logisticLinksCheckBox_->isChecked() || 
        name == "diplomacy" && !diplomacyCheckBox_->isChecked() )
    {
        xos << xml::start( name );
        xos << xml::end;
        return;
    }
    if( name == "party" )
    {
        unsigned long partyID = xis.attribute< unsigned long >( "id", 0 );
        if( partyID != 0 && !IsPartyChecked( partyID ) )
            return;
    }
    if( name == "diplomacy" )
    {
        xos << xml::start( name );
        xis >> xml::list( "party", *this, &FilterOrbatReIndexer::ReadDiplomacy, xos );
        xos << xml::end;
    }
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
// Name: FilterOrbatReIndexer::ReadPartyDiplomacy
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::ReadDiplomacy( xml::xistream& xis, xml::xostream& xos )
{
    unsigned long partyID = xis.attribute< unsigned long >( "id", 0 );
    if( partyID != 0 && IsPartyChecked( partyID ) )
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
    if( partyID != 0 && IsPartyChecked( partyID ) )
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
QWidget* FilterOrbatReIndexer::CreateParametersWidget( QWidget* parent )
{
    Q3GroupBox* parametersWidget = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Import parameters" ), parent, "FilterOrbatReIndexer_ParameterGroupBox" );
    {
        Q3HBox* hbox = new Q3HBox( parametersWidget, "FilterOrbatReIndexer_OrbatHBox" );
        hbox->setSpacing( 5 );
        new QLabel( tools::translate( "FilterOrbatReIndexer", "Select order of battle:" ), hbox, "FilterOrbatReIndexer_OrbatLabel" );
        filename_ = new QLineEdit( tools::translate( "FilterOrbatReIndexer", "Enter path to the new orbat here" ), hbox, "FilterOrbatReIndexer_OrbatLineEdit" );
        filename_->setReadOnly( true );
        QPushButton* browseBtn = new QPushButton( tools::translate( "FilterOrbatReIndexer", "Browse..." ), hbox, "FilterOrbatReIndexer_OrbatBrowseButton" );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( OnBrowse() ) );
    }
    {
        Q3HBox* hbox = new Q3HBox( parametersWidget, "FilterOrbatReIndexer_ParameterHBox" );

        Q3GroupBox* optionBox = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Import options:" ), hbox, "FilterOrbatReIndexer_PartiesGroupBox" );
        objectsCheckBox_       = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Objects" )       , "FilterOrbatReIndexer_ObjectsCheckBox" );
        populationsCheckBox_   = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Crowd" )         , "FilterOrbatReIndexer_PopulationsCheckBox");
        initialStateCheckBox_  = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Initial state" ) , "FilterOrbatReIndexer_InitialStateCheckBox");
        logisticLinksCheckBox_ = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Logistic links" ), "FilterOrbatReIndexer_LogisticLinksCheckBox");
        stocksCheckBox_        = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Stocks" )        , "FilterOrbatReIndexer_StocksCheckBox");
        diplomacyCheckBox_     = AddCheckBox( optionBox, tools::translate( "FilterOrbatReIndexer", "Diplomacy" )     , "FilterOrbatReIndexer_DiplomacyCheckBox");

        Q3GroupBox* sideBox = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterOrbatReIndexer", "Parties:" ), hbox, "FilterOrbatReIndexer_PartiesGroupBox" );
        partiesListView_ = new Q3ListView( sideBox, "FilterOrbatReIndexer_PartiesListView" );

        partiesListView_->addColumn( "Checkbox" );
        partiesListView_->addColumn( "Hidden checkbox", 0 );
        partiesListView_->addColumn( "Hidden party id", 0 );
        partiesListView_->addColumn( "Party" );
        partiesListView_->header()->setResizeEnabled( false );
        partiesListView_->header()->setMovingEnabled( false );
        partiesListView_->hideColumn( eHiddenPartyID );
        partiesListView_->hideColumn( eHiddenCheckbox );
        partiesListView_->header()->hide();

        partiesListView_->setResizeMode( Q3ListView::LastColumn );
        partiesListView_->setEnabled( false );

        connect( partiesListView_, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
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

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::OnItemClicked
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
void FilterOrbatReIndexer::OnItemClicked( Q3ListViewItem* item, const QPoint& /*point*/, int column )
{
    if( column != eCheckbox || !item )
        return;

    bool checked = item->text( eHiddenCheckbox ).toInt() > 0;
    checked = !checked;
    item->setText( eHiddenCheckbox, QString::number( ( checked ) ? 1 : 0 ) );
    item->setPixmap( eCheckbox, ( checked ) ? checkedPixmap_ : uncheckedPixmap_  );
}

// -----------------------------------------------------------------------------
// Name: FilterOrbatReIndexer::IsPartyChecked
// Created: ABR 2011-09-20
// -----------------------------------------------------------------------------
bool FilterOrbatReIndexer::IsPartyChecked( unsigned long partyID ) const
{
    for( Q3ListViewItemIterator it( partiesListView_ ); it.current(); ++it )
        if( ( *it )->text( eHiddenPartyID ).toUInt() == partyID )
            return ( *it )->text( eHiddenCheckbox ).toInt() > 0;
    assert( false );
    return false;
}
