// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DragAndDropHelpers_h_
#define __DragAndDropHelpers_h_

#include "StandardModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <QtGui/qevent.h>

namespace kernel
{
    class Controllers;
}

namespace dnd
{
    template< typename T >
    bool IsA( QStandardItem& item )
    {
        if( item.data( gui::Roles::MimeTypeRole ).isValid() )
        {
            std::string tmp1 = typeid( T ).name();
            std::string tmp2 = item.data( gui::Roles::MimeTypeRole ).toString();
            return typeid( T ).name() == item.data( gui::Roles::MimeTypeRole ).toString();
        }
        return typeid( T ).name() == typeid( QString ).name();
    }

    template< typename T >
    bool HasData( const QDropEvent* event )
    {
        if( !event )
            return false;
        const QMimeData* mimeData = event->mimeData();
        if( !mimeData )
            return false;
        QStringList formats = mimeData->formats();
        foreach( QString format, formats )
        {
            if( format == typeid( T ).name() )
                return true;
        }
        return false;
    }

    template< typename T >
    T* FindData( const QDropEvent* event )
    {
        if( !event )
            return 0;
        const QMimeData* mimeData = event->mimeData();
        if( !mimeData )
            return 0;
        QStringList formats = mimeData->formats();
        foreach( QString format, formats )
        {
            if( format == typeid( T ).name() )
            {
                QByteArray encodedData = mimeData->data( format );
                QDataStream stream( &encodedData, QIODevice::ReadOnly );
                if( !stream.atEnd() )
                {
                    unsigned int ptr = 0;
                    stream >> ptr;
                    if( ptr )
                        return reinterpret_cast< T* >( ptr );
                }
            }
        }
        return 0;
    }

    template< typename T >
    T* FindSafeData( QDropEvent* event )
    {
        if( !event )
            return 0;
        const QMimeData* mimeData = event->mimeData();
        if( !mimeData )
            return 0;
        QStringList formats = mimeData->formats();
        foreach( QString format, formats )
        {
            if( format == typeid( T ).name() )
            {
                QByteArray encodedData = mimeData->data( format );
                QDataStream stream( &encodedData, QIODevice::ReadOnly );
                if( !stream.atEnd() )
                {
                    unsigned int ptr = 0;
                    stream >> ptr;
                    kernel::SafePointer< T >* safePtr = reinterpret_cast< kernel::SafePointer< T >* >( ptr );
                    if( safePtr && *safePtr )
                        return safePtr->ConstCast();
                }
            }
        }
        return 0;
    }

    template< typename T1, typename T2, typename T3  >
    kernel::Entity_ABC* FindSafeEntityData( QDropEvent* event )
    {
        if( T1* data = FindSafeData< T1 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T2* data = FindSafeData< T2 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T3* data = FindSafeData< T3 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        return 0;
    }

    template< typename T1, typename T2, typename T3  >
    kernel::Entity_ABC* FindSafeEntityData( const gui::StandardModel& model, QStandardItem& item )
    {
        if( IsA< T1 >( item ) )
            return static_cast< kernel::Entity_ABC* >( model.GetDataFromItem< T1 >( item ) );
        if( IsA< T2 >( item ) )
            return static_cast< kernel::Entity_ABC* >( model.GetDataFromItem< T2 >( item ) );
        if( IsA< T3 >( item ) )
            return static_cast< kernel::Entity_ABC* >( model.GetDataFromItem< T3 >( item ) );
        return 0;
    }

    // tmp
    template< typename T1, typename T2, typename T3, typename T4, typename T5  >
    kernel::Entity_ABC* FindSafeEntityData( QDropEvent* event )
    {
        if( T1* data = FindSafeData< T1 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T2* data = FindSafeData< T2 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T3* data = FindSafeData< T3 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T4* data = FindSafeData< T4 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        if( T5* data = FindSafeData< T5 >( event ) )
            return static_cast< kernel::Entity_ABC* >( data );
        return 0;
    }

    template< typename T >
    QDrag* CreateDragObject( T* value, QWidget* parent )
    {
        QDrag* drag = new QDrag( parent );
        QMimeData* mimeData = new QMimeData();
        QByteArray encodedData;
        QDataStream stream( &encodedData, QIODevice::WriteOnly );
        stream << reinterpret_cast< unsigned int >( value );
        mimeData->setData( QString( typeid( T ).name() ), encodedData );
        drag->setMimeData( mimeData );
        drag->exec();
        return drag;
    }

    // tmp (memory leak, to be deleted once qt4 port is completed)
    template< typename T >
    QDrag* CreateSafeDragObject( T* value, QWidget* parent, kernel::Controllers& controllers )
    {
        QDrag* drag = new QDrag( parent );
        QMimeData* mimeData = new QMimeData();
        QByteArray encodedData;
        QDataStream stream( &encodedData, QIODevice::WriteOnly );
        kernel::SafePointer< T >* safe = new kernel::SafePointer< T >( controllers, value );
        stream << reinterpret_cast< unsigned int >( safe );
        mimeData->setData( QString( typeid( T ).name() ), encodedData );
        drag->setMimeData( mimeData );
        drag->exec();
        return drag;
    }

} // end namespace dnd

#endif // __DragAndDropHelpers_h_
