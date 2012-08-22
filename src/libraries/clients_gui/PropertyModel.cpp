// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyModel.h"
#include "PropertyItem.h"
#include "PropertyDisplayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyModel constructor
// Created: LGY 2012-08-09
// -----------------------------------------------------------------------------
PropertyModel::PropertyModel( gui::PropertyDisplayer& displayer )
    : displayer_( displayer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel destructor
// Created: LGY 2012-08-09
// -----------------------------------------------------------------------------
PropertyModel::~PropertyModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Clear
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::Clear()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Hide
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::SubItem
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& PropertyModel::SubItem( const QString& /*name*/ )
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::StartDisplay
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::StartDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::DisplayFormatted
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::DisplayFormatted( const QString& /*formatted*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::EndDisplay
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::EndDisplay()
{
    // NOTHING
}

namespace
{
    template< typename T >
    void CreateData( QStandardItem& item, int role, T data )
    {
        QVariant* variant = new QVariant();
        variant->setValue( data );
        item.setData( *variant, role );
    }

    void CreateRow( QStandardItem& parent, const QString& name, kernel::Property_ABC& property, gui::PropertyDisplayer& displayer )
    {
        QList< QStandardItem* > items;
        items << new QStandardItem( name );
        PropertyItem* item = new PropertyItem( property, displayer );
        CreateData( *item, Qt::UserRole, kernel::VariantPointer( &property ) );
        CreateData( *item, Qt::UserRole + 1, static_cast< unsigned int >( property.GetCategory() ) );
        items << item;
        parent.appendRow( items );
    }

    QStandardItem* CreateCategory( QStandardItem& parent, const QString& name )
    {
        QStandardItem* category = new QStandardItem( name );
        CreateData( *category, Qt::BackgroundRole, QBrush( QColor( 115, 110, 100 ) ) );
        QFont boldFont;
        boldFont.setBold( true );
        CreateData( *category, Qt::FontRole, boldFont );
        CreateData( *category, Qt::ForegroundRole, QBrush( QColor( 255, 255, 255 ) ) );
        QList< QStandardItem* > items;
        items << category;
        items << new QStandardItem();
        parent.appendRow( items );
        return category;
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::FindItem
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
QStandardItem* PropertyModel::FindItem( QStandardItem* item, const QString& text ) const
{
    for( int row = 0; row < item->rowCount(); ++row )
    {
        QStandardItem* childItem = item->child( row, 0 );
        if( childItem && childItem->text() == text)
            return childItem;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::FindItem
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
QStandardItem* PropertyModel::FindItem( const QString& category ) const
{
    QStandardItem* parent = invisibleRootItem();
    QStringList path = QStringList::split( '/', category );
    for( int i = 0; i < path.size(); ++i )
    {
        const QString name = path.at( i );
        QStandardItem* item = FindItem( parent, name );
        if( !item )
             return 0;
        if( i == path.size() - 1 )
            return item;
        parent = item;
    }
    return 0 ;
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Call
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyModel::Call( kernel::Property_ABC* const& property )
{
    QStandardItem* parent = invisibleRootItem();
    QStringList path = QStringList::split( '/', property->GetName() );
    for( int i = 0; i < path.size(); ++i )
    {
        const QString category = path.at( i );
        if( i == path.size() - 1 )
            CreateRow( *parent, category, *property, displayer_ );
        else
        {
            QStandardItem* item = FindItem( parent, category );
            parent = item ? item : CreateCategory( *parent, category );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Update
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyModel::Update( QWidget* editor, const QModelIndex& index )
{
    if( PropertyItem* item = static_cast< PropertyItem* >( itemFromIndex( index ) ) )
    {
        item->Update( editor );
        kernel::E_Category category = static_cast< kernel::E_Category >( item->data( Qt::UserRole + 1 ).toInt () );
        if( category != kernel::eNothing )
            Update( invisibleRootItem(), category );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Update
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyModel::Update( QStandardItem* parent, kernel::E_Category category )
{
    for( int i = 0; i< parent->rowCount(); ++i )
        if( QStandardItem* childItem = parent->child( i, 0 ) )
        {
            Update( childItem, category );
            if( PropertyItem* item = static_cast< PropertyItem* >( parent->child( i, 1 ) ) )
                if( static_cast< kernel::E_Category >( item->data( Qt::UserRole + 1 ).toInt () ) == category )
                    item->Update();
        }
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Update
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyModel::Update( const QString& category )
{
    QStandardItem* item = FindItem( category );
    if( item )
        Update( FindParent( item ), item );
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Update
// Created: LGY 2012-08-21
// -----------------------------------------------------------------------------
void PropertyModel::Update( QStandardItem* parent, QStandardItem* property )
{
    int childCount = property->rowCount();
    if( childCount > 0 )
        for( int i = 0; i < childCount; ++i )
            Update( property, property->child( i ) );
    else if( PropertyItem* item = static_cast< PropertyItem* >( parent->child( property->row(), 1 ) ) )
            item->Update();
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::Delete
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
void PropertyModel::Delete( const QString& category )
{
    QStandardItem* item = FindItem( category );
    if( item )
    {
        QList< QStandardItem* > rowItems = FindParent( item )->takeRow( item->row() );
        for( QList< QStandardItem *>::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
            delete *it;
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyModel::FindParent
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
QStandardItem* PropertyModel::FindParent( QStandardItem* item ) const
{
    return item->parent() ? item->parent() : invisibleRootItem();
}
