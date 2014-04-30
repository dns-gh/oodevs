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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/RichWidget.h"
#include "preparation/InitialState.h"
#include <boost/smart_ptr/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: UnitStateDialog constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateDialog::UnitStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel,
                                  gui::DisplayExtractor& extractor )
    : gui::UnitStateDialog( parent, controllers )
{
    gui::SubObjectName subObject( "UnitStateDialog" );
    assert( tabWidget_ );
    tabs_.push_back( boost::make_shared< UnitStateTableCrew >( tabWidget_, controllers ) );
    tabs_.push_back( boost::make_shared< UnitStateTableEquipment >( tabWidget_, extractor, controllers ) );
    tabs_.push_back( boost::make_shared< UnitStateTableResource > ( tabWidget_, staticModel, controllers ) );
    tabWidget_->addTab( tabs_[ eCrew      ].get(), tools::translate( "UnitStateDialog", "Crew" ) );
    tabWidget_->addTab( tabs_[ eEquipment ].get(), tools::translate( "UnitStateDialog", "Equipments" ) );
    tabWidget_->addTab( tabs_[ eSupplies ].get(), tools::translate( "UnitStateDialog", "Resources" ) );
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

namespace
{
    void RecursiveReset( kernel::Entity_ABC& entity, kernel::Controller& controller )
    {
        if( entity.GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            InitialState* extension = entity.Retrieve< InitialState >();
            if( extension )
            {
                extension->Reset();
                controller.Update( entity );
            }
        }
        else
        {
            const kernel::TacticalHierarchies& hierarchy = entity.Get< kernel::TacticalHierarchies >();
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& subEntity = it.NextElement();
                RecursiveReset( const_cast< kernel::Entity_ABC& >( subEntity ), controller );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Reset
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateDialog::Reset()
{
    RecursiveReset( *selected_.ConstCast(), controllers_.controller_ );
    gui::UnitStateDialog::Reset();
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifySelected
// Created: ABR 2011-08-31
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ == element )
        return;
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
    menu.InsertItem( "Helpers", tools::translate( "UnitStateDialog", "Change initial state" ), this, SLOT( Show() ), false, 5 );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Automat_ABC* > ( &entity );
    menu.InsertItem( "Helpers", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ), false, 5 );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Formation_ABC* > ( &entity );
    menu.InsertItem( "Helpers", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ), false, 5 );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifyContextMenu
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    if( entity.GetId() == 0 ) // no side team
        return;
    selected_ = const_cast< kernel::Team_ABC* > ( &entity );
    menu.InsertItem( "Helpers", tools::translate( "UnitStateDialog", "Display initial state" ), this, SLOT( Show() ), false, 5 );
}
