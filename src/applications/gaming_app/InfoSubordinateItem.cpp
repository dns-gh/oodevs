// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSubordinateItem.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/EntitySymbols.h"
#include "gaming/Attributes.h"

Q_DECLARE_METATYPE( const kernel::Entity_ABC* );

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem constructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::InfoSubordinateItem( QListWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons, const kernel::Entity_ABC& entity )
    : QListWidgetItem( entity.GetName(), parent )
    , controllers_( controllers )
    , icons_( icons )
    , oldRawState_( -1 )
{
    setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    setData( Qt::UserRole + 1, QVariant::fromValue( &entity ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem destructor
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
InfoSubordinateItem::~InfoSubordinateItem()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::data
// Created: JSR 2012-10-15
// -----------------------------------------------------------------------------
QVariant InfoSubordinateItem::data( int role ) const
{
    if( role == Qt::DecorationRole )
    {
        assert( listWidget() );
        const QPixmap& p = icons_.GetSymbol( GetEntity() );
        if( p.isNull() )
            QTimer::singleShot( 0, listWidget(), SLOT( doItemsLayout() ) );
        return QIcon( p );
    }
    return QListWidgetItem::data( role );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::NotifyUpdated
// Created: SBO 2007-02-23
// -----------------------------------------------------------------------------
void InfoSubordinateItem::NotifyUpdated( const Attributes& attributes )
{
    if( &attributes == GetEntity().Retrieve< Attributes >() )
    {
        int opState = static_cast< const Attributes& >( attributes ).nRawOpState_;
        if( opState != oldRawState_ )
        {
            oldRawState_ = opState;
            QTimer::singleShot( 0, listWidget(), SLOT( doItemsLayout() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::GetEntity
// Created: SBO 2007-02-22
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& InfoSubordinateItem::GetEntity() const
{
    return *data( Qt::UserRole + 1 ).value< const kernel::Entity_ABC* >();
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinateItem::NotifyUpdated
// Created: ABR 2014-09-10
// -----------------------------------------------------------------------------
void InfoSubordinateItem::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( &entity == &GetEntity() )
        setText( entity.GetName() );
}
