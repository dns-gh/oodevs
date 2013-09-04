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
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichTreeWidget.h"

Q_DECLARE_METATYPE( const indicators::Primitive* )

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesPage::ScorePrimitivesPage( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const indicators::Primitives& primitives, const T_Filter& filter )
    : Q3VBox( parent )
    , controllers_( controllers )
    , filter_( filter )
    , primitives_( primitives )
{
    gui::SubObjectName subObject( objectName );
    {
        list_ = new gui::RichTreeWidget( "list", this );
        list_->setRootIsDecorated( false );
        list_->setHeaderHidden( true );
        connect( list_, SIGNAL( itemSelectionChanged() ), SLOT( OnSelectionChanged() ) );
        connect( list_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnInsert() ) );
    }
    {
        gui::RichPushButton* insert = new gui::RichPushButton( "insert", tr( "Insert" ), this );
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
// Name: ScorePrimitivesPage::OnInsert
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::OnInsert()
{
     if( QTreeWidgetItem* item = list_->currentItem() )
         if( const indicators::Primitive* primitive = item->data( 0, Qt::UserRole + 1 ).value< const indicators::Primitive* >() )
            emit Insert( primitive->GetPrototype() );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const indicators::Primitive& > it( primitives_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const indicators::Primitive& element = it.NextElement();
        if( filter_( element ) )
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText( 0, element.GetName() );
            item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( &element ) );
            list_->addTopLevelItem( item );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    list_->model()->removeRows( 0, list_->model()->rowCount() );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesPage::OnSelectionChanged
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScorePrimitivesPage::OnSelectionChanged()
{
    if( QTreeWidgetItem* item = list_->currentItem() )
        if( const indicators::Primitive* primitive = item->data( 0, Qt::UserRole + 1 ).value< const indicators::Primitive* >() )
            emit Selected( *primitive );
}
