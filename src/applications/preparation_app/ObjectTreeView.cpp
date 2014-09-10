// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectTreeView.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/AltitudeModifierAttribute.h"
#include "preparation/FloodAttribute.h"

namespace
{
    class ObjectMimeData : public QMimeData
    {
    public:
        ObjectMimeData( const kernel::Entity_ABC& entity )
            : entity_( entity )
        {
            if( const AltitudeModifierAttribute* attribute = entity_.Retrieve< AltitudeModifierAttribute >() )
                attribute->BeginDrag();
        }
        virtual ~ObjectMimeData()
        {
            if( const AltitudeModifierAttribute* attribute = entity_.Retrieve< AltitudeModifierAttribute >() )
                attribute->EndDrag();
            if( const FloodAttribute* flood = entity_.Retrieve< FloodAttribute >() )
                flood->EndDrag();
        }
    private:
        const kernel::Entity_ABC& entity_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView constructor
// Created: JSR 2012-09-20
// -----------------------------------------------------------------------------
ObjectTreeView::ObjectTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : gui::ObjectTreeView( objectName, controllers, profile, modelObserver, parent)
{
    EnableDragAndDrop( true );
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView destructor
// Created: JSR 2012-09-20
// -----------------------------------------------------------------------------
ObjectTreeView::~ObjectTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::MimeTypes
// Created: JSR 2012-09-20
// -----------------------------------------------------------------------------
QStringList ObjectTreeView::MimeTypes() const
{
    return QStringList( typeid( kernel::Object_ABC ).name() );
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::ItemSpecificFlags
// Created: JSR 2012-09-20
// -----------------------------------------------------------------------------
Qt::ItemFlags ObjectTreeView::ItemSpecificFlags( const kernel::Entity_ABC& entity ) const
{
    return gui::ObjectTreeView::ItemSpecificFlags( entity ) | Qt::ItemIsDragEnabled;
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::MimeData
// Created: JSR 2012-09-20
// -----------------------------------------------------------------------------
QMimeData* ObjectTreeView::MimeData( const QModelIndexList& indexes, bool& overriden ) const
{
    overriden = true;
    QStandardItem* item = 0;
    foreach( QModelIndex index, indexes )
    {
        if( index.isValid() )
        {
            item = dataModel_.itemFromIndex( index.model() == &dataModel_ ? index : proxyModel_->mapToSource( index ) );
            if( item )
                break;
        }
    }
    if( !item )
        return 0;
    kernel::Object_ABC* obj = dataModel_.GetDataFromItem< kernel::Object_ABC >( *item );
    if( !obj )
        return 0;
    ObjectMimeData* mimeData = new ObjectMimeData( *obj );
    QByteArray encodedData;
    QDataStream stream( &encodedData, QIODevice::WriteOnly );
    stream << reinterpret_cast< unsigned int >( item->data( gui::Roles::DataRole ).value< kernel::VariantPointer >().ptr_ );
    mimeData->setData( typeid( kernel::Object_ABC ).name(), encodedData );
    return mimeData;
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::keyPressEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void ObjectTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && event->key() == Qt::Key_Delete && index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            modelObserver_.DeleteEntity( *entity );
    }
    else
        gui::ObjectTreeView::keyPressEvent( event );
}
