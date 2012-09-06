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
#include "clients_kernel/Property_ABC.h"
#include "clients_kernel/VariantPointer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyDelegate constructor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
PropertyDelegate::PropertyDelegate( kernel::EditorFactory_ABC& factory )
    : factory_( factory )
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
    if( const kernel::Property_ABC* property = static_cast< const kernel::Property_ABC* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ ) )
        return const_cast< kernel::Property_ABC* >( property )->CreateEditor( parent, factory_ );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate::setModelData
// Created: LGY 2012-08-14
// -----------------------------------------------------------------------------
void PropertyDelegate::setModelData( QWidget* editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if( PropertyModel* standardModel = static_cast< PropertyModel* >( model ) )
        standardModel->Update( editor, index );
}

// -----------------------------------------------------------------------------
// Name: PropertyDelegate::sizeHint
// Created: LGY 2012-09-06
// -----------------------------------------------------------------------------
QSize PropertyDelegate::sizeHint( const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/ ) const
{
    return QSize( 50, 20 );
}
