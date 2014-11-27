// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyDelegate.h"
#include "PropertyModel.h"
#include "Property_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/VariantPointer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyDelegate constructor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
PropertyDelegate::PropertyDelegate( kernel::ActionController& actionController, EditorFactory_ABC& factory )
    : actionController_( actionController )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate destructor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
PropertyDelegate::~PropertyDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate::createEditor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
QWidget* PropertyDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
{
    if( const Property_ABC* property = static_cast< const Property_ABC* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ ) )
        return const_cast< Property_ABC* >( property )->CreateEditor( parent, factory_ );
    return 0;
}

void PropertyDelegate::setEditorData( QWidget*, const QModelIndex& ) const
{
    // We need an empty implementation of setEditorData because QItemDelegate's
    // implementation produces a warning if used with our own QComboBox.
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate::setModelData
// Created: LGY 2012-08-14
// -----------------------------------------------------------------------------
void PropertyDelegate::setModelData( QWidget* editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if( PropertyModel* standardModel = static_cast< PropertyModel* >( model ) )
    {
        actionController_.BlockSelection( true );

        standardModel->Update( editor, index );

        actionController_.BlockSelection( false );
        QMetaObject::invokeMethod( standardModel, "InternalItemChanged" );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate::sizeHint
// Created: LGY 2012-09-06
// -----------------------------------------------------------------------------
QSize PropertyDelegate::sizeHint( const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const
{
    return QSize( 50, 20 );
}
