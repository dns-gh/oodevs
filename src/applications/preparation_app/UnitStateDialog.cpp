// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitStateDialog.h"
#include "moc_UnitStateDialog.cpp"
#include "UnitStateTableCrew.h"
#include "UnitStateTableEquipment.h"
#include "UnitStateTableResource.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/InitialState.h"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: UnitStateDialog constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateDialog::UnitStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : gui::UnitStateDialog( parent, controllers )
{
    setCaption( tools::translate( "UnitStateDialog", "Initial state" ) );
    assert( tabWidget_ );
    exportButton_ = new QPushButton( tr( "Export" ) );
    buttons_->addWidget( exportButton_, 0, 2 );
    tabs_.push_back( boost::shared_ptr< UnitStateTableCrew >     ( new UnitStateTableCrew(      tabWidget_ ) ) );
    tabs_.push_back( boost::shared_ptr< UnitStateTableEquipment >( new UnitStateTableEquipment( tabWidget_ ) ) );
    tabs_.push_back( boost::shared_ptr< UnitStateTableResource > ( new UnitStateTableResource(  tabWidget_, staticModel ) ) );
    tabWidget_->addTab( tabs_[ eCrew      ].get(), tools::translate( "UnitStateDialog", "Crew" ) );
    tabWidget_->addTab( tabs_[ eEquipment ].get(), tools::translate( "UnitStateDialog", "Equipments" ) );
    tabWidget_->addTab( tabs_[ eResources ].get(), tools::translate( "UnitStateDialog", "Resources" ) );
    connect( exportButton_, SIGNAL( clicked() ), SLOT( Export() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateDialog::~UnitStateDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Validate
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::Validate() const
{
    gui::UnitStateDialog::Validate();
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Reset
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateDialog::Reset()
{
    InitialState& extension = selected_.ConstCast()->Get< InitialState >();
    extension.Reset();
    gui::UnitStateDialog::Reset();
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifySelected
// Created: ABR 2011-08-31
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->SetReadOnly( false );
    gui::UnitStateDialog::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Agent_ABC* > ( &entity );
    menu.InsertItem( "Update", tools::translate( "UnitStateDialog", "Change initial state" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Automat_ABC* > ( &entity );
    menu.InsertItem( "Update", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Formation_ABC* > ( &entity );
    menu.InsertItem( "Update", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Team_ABC* > ( &entity );
    menu.InsertItem( "Update", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Export
// Created: LGY 2012-01-30
// -----------------------------------------------------------------------------
void UnitStateDialog::Export()
{
    if( selected_ )
    {
        std::string fileName = QFileDialog::getExistingDirectory( this, tools::translate( "UnitStateDialog", "Export unit state" ) ).toStdString();
        if( fileName != "" )
        {
            bfs::path path( fileName / bfs::path( selected_->GetName() ) );
            if( bfs::create_directories( path ) )
            {
                tabs_[ eCrew ]->Serialize( bfs::path( path / bfs::path( tools::translate( "UnitStateDialog", "crew" ) + ".csv" ).filename() ) );
                tabs_[ eEquipment ]->Serialize( bfs::path( path / bfs::path( tools::translate( "UnitStateDialog", "equipments" ) + ".csv" ).filename() ) );
                tabs_[ eResources ]->Serialize( bfs::path( path / bfs::path( tools::translate( "UnitStateDialog", "resources" ) + ".csv" ).filename() ) );
            }
        }
    }
}
