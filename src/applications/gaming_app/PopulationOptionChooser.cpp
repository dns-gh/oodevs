// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationOptionChooser.h"
#include "moc_PopulationOptionChooser.cpp"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/ChangePopulationDisplay.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include "gaming/StaticModel.h"
#include <boost/assign.hpp>

namespace
{
    enum E_PopulationOption
    {
        eNone,
        eDensity,
        eAccommodation,
    };
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser constructor
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
PopulationOptionChooser::PopulationOptionChooser( QMainWindow* parent, kernel::Controllers& controllers, StaticModel& staticModel )
    : QDialog( parent, "population-display-option", false, Qt::FramelessWindowHint )
    , controllers_( controllers )
    , accomodations_( staticModel.accommodationTypes_ )
{
    setObjectName( "popDisplayOption" );
    setWindowTitle( tools::translate( "PopulationOptionChooser", "Population Display Options" ) );
    qApp->installEventFilter( this );

    displayCombo_ = new QComboBox();
    const std::map< E_PopulationOption, QString > values = boost::assign::map_list_of( eNone, tr( "Off" ) )
                                                                                     ( eDensity, tr( "Density" ) )
                                                                                     ( eAccommodation, tr( "Occupation" ) );
    for( auto it = values.begin(); it != values.end(); ++it )
        displayCombo_->insertItem( it->first, it->second );
    displayCombo_->setCurrentItem( 0 );
    displayCombo_->setEditable( false );

    activityCombo_ = new QComboBox();
    activityCombo_->setEditable( false );
    activityCombo_->setEnabled( false );

    occupationList_ = new QListWidget();
    occupationList_->setEnabled( false );

    QWidget* panel = new QWidget( this );
    QVBoxLayout* panelLayout = new QVBoxLayout( panel );
    panelLayout->addWidget( new QLabel(), 0 );
    panelLayout->addWidget( displayCombo_, 0 );
    panelLayout->addWidget( activityCombo_, 0 );
    panelLayout->addWidget( occupationList_, 0 );

    connect( displayCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnDisplayChanged( int ) ) );
    connect( activityCombo_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnActivityChanged( int ) ) );
    connect( occupationList_, SIGNAL( itemClicked ( QListWidgetItem* ) ), this, SLOT( OnOccupationChanged( QListWidgetItem* ) ) );

    hide();
    resize( 280, 300 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser destructor
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
PopulationOptionChooser::~PopulationOptionChooser()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnDisplayChanged
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnDisplayChanged( int index )
{
    controllers_.options_.Change( "Density/Enabled", index == eDensity );
    const bool accommodationEnabled = index == eAccommodation;
    controllers_.options_.Change( "Accommodation/Enabled", accommodationEnabled );
    activityCombo_->setEnabled( accommodationEnabled );
    occupationList_->setEnabled( accommodationEnabled );
    if( accommodationEnabled )
    {
        int index = activityCombo_->findText( controllers_.options_.GetOption( "Accommodation/Displayed" ).To< QString >() );
        activityCombo_->setCurrentIndex( index < 0 ? 0 : index );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnActivityChanged
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnActivityChanged( int index )
{
    controllers_.options_.Change( "Accommodation/Displayed", activityCombo_->itemText( index ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnOccupationChanged
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnOccupationChanged( QListWidgetItem* item )
{
    if( item )
    {
        kernel::ChangePopulationDisplay display( item->text().toStdString(), item->checkState() == Qt::Checked );
        controllers_.controller_.Update( display );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::Show
// Created: NPT 2012-10-15
// -----------------------------------------------------------------------------
void PopulationOptionChooser::Show()
{
    const QPushButton* button = dynamic_cast< const QPushButton* >( sender() );
    if( !button )
        return;
    move( button->mapToGlobal( button->pos() ).x() - 3*button->width(), button->mapToGlobal( button->pos() ).y() + button->height() );
    show();
    displayCombo_->setCurrentIndex( controllers_.options_.GetOption( "Density/Enabled" ).To< bool >() ? eDensity :
                                    controllers_.options_.GetOption( "Accommodation/Enabled" ).To< bool >() ? eAccommodation : eNone );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyUpdated
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyUpdated( const kernel::ModelLoaded& )
{
    activityCombo_->clear();
    auto it = accomodations_.CreateIterator();
    while( it.HasMoreElements() )
        activityCombo_->addItem( it.NextElement().GetRole().c_str() );
    if( activityCombo_->count() > 0 )
        activityCombo_->setCurrentItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyUpdated
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    if( !isVisible() )
        hide();
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyCreated
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyCreated( const kernel::Inhabitant_ABC& inhabitant )
{
    QListWidgetItem* item = new QListWidgetItem( inhabitant.GetName() );
    item->setCheckState( Qt::Checked );
    OnOccupationChanged( item );
    occupationList_->addItem( item );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OptionChanged
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( displayCombo_ )
        if( name == "Density/Enabled" )
            if( value.To< bool >() )
                displayCombo_->setCurrentIndex( eDensity );
        else if( name == "Accommodation/Enabled" )
            if( value.To< bool >() )
                displayCombo_->setCurrentIndex( eAccommodation );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::eventFilter
// Created: NPT 2012-10-16
// -----------------------------------------------------------------------------
bool PopulationOptionChooser::eventFilter( QObject*, QEvent* e )
{
    switch( e->type() )
    {
    case QEvent::MouseButtonPress:
        if( ! rect().contains( mapFromGlobal( static_cast<QMouseEvent*>( e )->globalPos() ) ) )
            hide();
    }
    return false;
}
