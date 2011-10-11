// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitStateDialog.h"
#include "moc_UnitStateDialog.cpp"
#include "UnitStateTableCrew.h"
#include "UnitStateTableEquipment.h"
#include "UnitStateTableResource.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/Equipments.h"
#include "gaming/Troops.h"

// -----------------------------------------------------------------------------
// Name: UnitStateDialog constructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateDialog::UnitStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : gui::UnitStateDialog( parent, controllers )
    , profile_         ( profile )
    , disconnected_    ( false )
    , resourceToolTip_ ( tr( "Resources" ) )
    , equipmentToolTip_( tr( "Composition" ) )
{
    setCaption( "Unit state" );
    assert( tabWidget_ );
    tabs_.push_back( boost::shared_ptr< UnitStateTableCrew >     ( new UnitStateTableCrew(      controllers, staticModel, actionsModel, simulation, tabWidget_ ) ) );
    tabs_.push_back( boost::shared_ptr< UnitStateTableEquipment >( new UnitStateTableEquipment( controllers, staticModel, actionsModel, simulation, tabWidget_ ) ) );
    tabs_.push_back( boost::shared_ptr< UnitStateTableResource > ( new UnitStateTableResource(  controllers, staticModel, actionsModel, simulation, tabWidget_ ) ) );
    tabWidget_->addTab( tabs_[ eCrew      ].get(), tr( "Crew" ) );
    tabWidget_->addTab( tabs_[ eEquipment ].get(), tr( "Equipments" ) );
    tabWidget_->addTab( tabs_[ eResources ].get(), tr( "Resources" ) );
    connect( tabWidget_, SIGNAL( currentChanged( QWidget* ) ), SLOT( OnTabChanged( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog destructor
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
UnitStateDialog::~UnitStateDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::GetEquipmentToolTip
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
const QString& UnitStateDialog::GetEquipmentToolTip() const
{
    return equipmentToolTip_;
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::GetResourceToolTip
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
const QString& UnitStateDialog::GetResourceToolTip() const
{
    return resourceToolTip_;
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::CheckCurrent
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
bool UnitStateDialog::CheckCurrent( E_Tabs tab ) const
{
    assert( tabWidget_->currentPageIndex() < eNbTabs );
    E_Tabs current = static_cast< E_Tabs >( tabWidget_->currentPageIndex() );
    if( current == eCrew || current == eEquipment )
        return tab == eEquipment;
    else if( current == eResources )
        return tab == eResources;
    else
        assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::EmitSignal
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::EmitSignal( bool state, E_Tabs tab )
{
    emit ( tab == eResources ) ? OnToggleResource( state ) : OnToggleEquipment( state );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::EmitSignals
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::EmitSignals()
{
    EmitSignal( CheckCurrent( eResources ), eResources );
    EmitSignal( CheckCurrent( eEquipment ), eEquipment );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::EmitCloseCurrentSignal
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::EmitCloseCurrentSignal()
{
    assert( tabWidget_->currentPageIndex() < eNbTabs );
    E_Tabs current = static_cast< E_Tabs >( tabWidget_->currentPageIndex() );
    disconnected_ = true;
    EmitSignal( false, current );
    disconnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::OnToggle
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::Toggle( E_Tabs tab )
{
    if( !disconnected_ )
    {
        disconnected_ = true;
        if( isShown() )
        {
            if( CheckCurrent( tab ) )
                hide();
            else
            {
                tabWidget_->setCurrentPage( tab );
                EmitSignals();
            }
        }
        else
        {
            Show();
            tabWidget_->setCurrentPage( tab );
            EmitSignals();
        }
        disconnected_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::ToggleResource
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateDialog::ToggleResource( bool )
{
    Toggle( eResources );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::ToggleEquipment
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateDialog::ToggleEquipment( bool )
{
    Toggle( eEquipment );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::OnTabChanged
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::OnTabChanged( QWidget* )
{
    if( !disconnected_ )
    {
        disconnected_ = true;
        EmitSignals();
        disconnected_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::hideEvent
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateDialog::hideEvent( QHideEvent* event )
{
    EmitCloseCurrentSignal();
    gui::UnitStateDialog::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::closeEvent
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::closeEvent( QCloseEvent * event )
{
    EmitCloseCurrentSignal();
    gui::UnitStateDialog::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifySelected
// Created: ABR 2011-07-07
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ == element )
        return;
    bool readOnly = ( element ) ? !profile_.CanDoMagic( *element ) : true;
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->SetReadOnly( readOnly );
    bool enabled = element && element->Retrieve< Equipments >() != 0 && element->Retrieve< kernel::Dotations_ABC >() != 0 && element->Retrieve< Troops >() != 0;
    emit Disabled( !enabled );
    gui::UnitStateDialog::NotifySelected( element );
}
