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
#include "indicators/Primitive.h"
#include "indicators/Primitives.h"

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesPage::ScorePrimitivesPage( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& primitives, const T_Filter& filter )
    : Q3VBox( parent )
    , controllers_( controllers )
    , filter_( filter )
    , primitives_( primitives )
    , list_( new gui::ListDisplayer< ScorePrimitivesPage >( this, *this, factory ) )
{
    {
        list_->AddColumn( tr( "Name" ) );
        list_->header()->hide();
        connect( list_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChanged() ) );
        connect( list_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnInsert() ) );
    }
    {
        QPushButton* insert = new QPushButton( tr( "Insert" ), this );
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
void ScorePrimitivesPage::Display( const indicators::Primitive& primitive, kernel::Displayer_ABC& /*displayer*/, gui::ValuedListItem* item )
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
        if( const indicators::Primitive* primitive = item->GetValue< indicators::Primitive >() )
            emit Insert( primitive->GetPrototype() );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Resolver< const indicators::Primitive, QString > subset;
    tools::Iterator< const indicators::Primitive& > it( primitives_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const indicators::Primitive& element = it.NextElement();
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
void ScorePrimitivesPage::OnSelectionChanged()
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( list_->selectedItem() ) )
        if( const indicators::Primitive* primitive = item->GetValue< indicators::Primitive >() )
            emit Selected( *primitive );
}
