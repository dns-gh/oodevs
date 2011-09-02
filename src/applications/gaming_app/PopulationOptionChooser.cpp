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
#include "clients_gui/ChangePopulationDisplay.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Tools.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser constructor
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
PopulationOptionChooser::PopulationOptionChooser( QMainWindow* parent, kernel::Controllers& controllers, StaticModel& staticModel )
    : QDockWidget( "population-display-option", parent )
    , controllers_( controllers )
    , accomodations_( staticModel.accommodationTypes_ )
{
    setObjectName( "popDisplayOption" );
    setCaption( tools::translate( "PopulationOptionChooser", "Population Display Options" ) );
    {
        Q3HBox* box = new Q3VBox( this );
        Q3ButtonGroup* group = new Q3ButtonGroup( 1, Qt::Horizontal, box );
        group->setExclusive( true );
        QRadioButton* off = new QRadioButton( group );
        off->setText( tools::translate( "PopulationOptionChooser", "Off" ) );
        off->setChecked( true );
        density_ = new QRadioButton( group );
        density_->setText( tools::translate( "PopulationOptionChooser", "Density" ) );
        occupation_ = new QRadioButton( group );
        occupation_->setText( tools::translate( "PopulationOptionChooser", "Occupation" ) );
        connect( density_, SIGNAL( toggled( bool ) ), this, SLOT( OnDensityToggled( bool ) ) );
        connect( occupation_, SIGNAL( toggled( bool ) ), this, SLOT( OnOccupationToggled( bool ) ) );
        activities_ = new Q3ButtonGroup( 1, Qt::Horizontal, box );
        activities_->setExclusive( true );
        activities_->setEnabled( false );
        populations_ = new Q3ButtonGroup( 1, Qt::Horizontal, box );
        populations_->setEnabled( false );
        setWidget( box );
    }
    setFeatures( QDockWidget::AllDockWidgetFeatures );
    hide();
    setProperty( "notAppropriate", QVariant( true ) );
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
// Name: PopulationOptionChooser::OnDensityToggled
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnDensityToggled( bool toggled )
{
    controllers_.options_.Change( "UrbanDensityColor", toggled );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnOccupationToggled
// Created: LDC 2011-03-23
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnOccupationToggled( bool toggled )
{
    activities_->setEnabled( toggled );
    populations_->setEnabled( toggled );
    controllers_.options_.Change( "UrbanAccommodationColor", toggled );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyUpdated
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const kernel::AccommodationType& > it = accomodations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        QRadioButton* next = new QRadioButton( activities_ );
        next->setText( it.NextElement().GetRole().c_str() );
        connect( next, SIGNAL( toggled( bool ) ), this, SLOT( OnAccomodationToggled( bool ) ) );
    }
    const QObjectList buttons = activities_->children();
    for( QObjectList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
        if( QAbstractButton* button = dynamic_cast< QAbstractButton* >( *it ) )
        {
            button->toggle();
            break;
        }
    activities_->adjustSize();
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyUpdated
// Created: LDC 2011-03-24
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    hide();
    const QObjectList buttons = activities_->children();
    for( QObjectList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
    {
        QAbstractButton* button = dynamic_cast< QAbstractButton* >( *it );
        if( button )
            button->deleteLater();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyCreated
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyCreated( const kernel::Inhabitant_ABC& inhabitant )
{
    QCheckBox* newPopulation = new QCheckBox( populations_ );
    QString name = inhabitant.GetName();
    if( name.isNull() )
        name = "";
    newPopulation->setText( name );
    connect( newPopulation, SIGNAL( toggled( bool ) ), this, SLOT( OnPopulationToggled( bool ) ) );
    newPopulation->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::NotifyDeleted
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void PopulationOptionChooser::NotifyDeleted( const kernel::Inhabitant_ABC& inhabitant )
{
    const QObjectList buttons = populations_->children();
    for( QObjectList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
    {
        QAbstractButton* button = dynamic_cast< QAbstractButton* >( *it );
        if( button && button->text() == inhabitant.GetName() )
            button->deleteLater();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnAccomodationToggled
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnAccomodationToggled( bool toggled )
{
    if( toggled )
    {
        const QAbstractButton* senderObject = dynamic_cast< const QAbstractButton* >( sender() );
        if( senderObject )
            controllers_.options_.Change( "UrbanAccommodationDisplayed", senderObject->text() );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OnPopulationToggled
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OnPopulationToggled( bool toggled )
{
    const QAbstractButton* senderObject = dynamic_cast< const QAbstractButton* >( sender() );
    if( senderObject )
    {
        gui::ChangePopulationDisplay display( senderObject->text().ascii(), toggled );
        controllers_.controller_.Update( display );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationOptionChooser::OptionChanged
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void PopulationOptionChooser::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( density_ && occupation_ )
    {
        if( name == "UrbanDensityColor" )
        {
            if( value.To< bool >() )
            {
                occupation_ = 0;
                density_->setChecked( true );
                density_ = 0;
            }
        }
        else if( name == "UrbanAccommodationColor" )
        {
            if( value.To< bool >() )
            {
                density_ = 0;
                occupation_->setChecked( true );
                occupation_ = 0;
            }
        }
    }
}
