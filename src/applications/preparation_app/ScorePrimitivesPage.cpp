// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScorePrimitivesPage.h"
#include "moc_ScorePrimitivesPage.cpp"
#include "clients_kernel/Controllers.h"
#include "preparation/IndicatorPrimitive.h"
#include "preparation/IndicatorPrimitives.h"

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesPage::ScorePrimitivesPage( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const IndicatorPrimitives& primitives, const T_Filter& filter )
    : QVBox( parent )
    , controllers_( controllers )
    , filter_( filter )
    , primitives_( primitives )
    , list_( new gui::ListDisplayer< ScorePrimitivesPage >( this, *this, factory ) )
{
    {
        list_->AddColumn( tr( "Name" ) );
        list_->header()->hide();
        connect( list_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChanged( QListViewItem* ) ) );
        connect( list_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnInsert() ) );
    }
    {
        QButton* insert = new QPushButton( tr( "Insert" ), this );
        connect( insert, SIGNAL( clicked() ), SLOT( OnInsert() ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesPage::~ScorePrimitivesPage()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::Display
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::Display( const IndicatorPrimitive& primitive, kernel::Displayer_ABC& /*displayer*/, gui::ValuedListItem* item )
{
    item->SetNamed( primitive );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::OnInsert
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::OnInsert()
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( list_->selectedItem() ) )
        if( const IndicatorPrimitive* primitive = item->GetValue< IndicatorPrimitive >() )
            emit Insert( primitive->GetPrototype() );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::NotifyUpdated( const kernel::ModelLoaded& )
{
    kernel::Resolver< const IndicatorPrimitive, QString > subset;
    kernel::Iterator< const IndicatorPrimitive& > it( primitives_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const IndicatorPrimitive& element = it.NextElement();
        if( filter_( element ) )
            subset.Register( element.GetName(), element );
    }
    list_->DeleteTail( list_->DisplayList( subset.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    list_->clear();
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::OnSelectionChanged
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::OnSelectionChanged( QListViewItem* item )
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( list_->selectedItem() ) )
        if( const IndicatorPrimitive* primitive = item->GetValue< IndicatorPrimitive >() )
            emit Selected( *primitive );
}
