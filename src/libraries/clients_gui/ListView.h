// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListView_h_
#define __ListView_h_

#include "tools/Iterator.h"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include "clients_kernel/ReadOnlyModable.h"

#pragma warning( push, 0 )
#include <QtGui/QWindowsStyle>
#pragma warning( pop )

namespace gui
{
class RichListView : public Q3ListView
                   , public kernel::ReadOnlyModable
{
    Q_OBJECT

public:
    explicit RichListView( QWidget* parent, const char* name = 0 )
        : Q3ListView( parent, name )
        , creationBlocked_   ( false )
        , dndLocked_         ( false )
        , contextMenuBlocked_( false )
    {
        // NOTHING
    }

    virtual ~RichListView()
    {
        // NOTHING
    }

public:
    void SetCreationBlocked( bool creationBlocked )
    {
        creationBlocked_ = creationBlocked;
    }

    bool IsCreationBlocked() const
    {
        return creationBlocked_;
    }

    void SetContextMenuBlocked( bool contextMenuBlocked )
    {
        contextMenuBlocked_ = contextMenuBlocked;
    }

    bool IsContextMenuBlocked() const
    {
        return contextMenuBlocked_;
    }

    void DeleteTail( ValuedListItem* item )
    {
        while( item )
        {
            ValuedListItem* next = (ValuedListItem*) item->nextSibling();
            delete item;
            item = next;
        }
    }

    void Purge()
    {
        DeleteTail( static_cast< ValuedListItem *>( firstChild() ) );
        clear();
    }

public slots:
    void SearchAndSelect( const QString& searchedText )
    {
        searchedText_ = searchedText;
        if( searchedText_.isEmpty() )
            return;

        Q3ListViewItem* selection = selectedItem();
        if( MatchItem( selection ) )
            return;
        else
            SearchAndSelectNext();
    }

    void SearchAndSelectNext()
    {
        if( searchedText_.isEmpty() )
            return;

        Q3ListViewItem* result = 0;
        Q3ListViewItem* selection = selectedItem();
        if( selection ) // Find next
        {
            Q3ListViewItemIterator it = firstChild();
            for( ; it.current(); ++it )
            {
                Q3ListViewItem* current = it.current();
                if( current == selection )
                {
                    ++it;
                    break;
                }
            }
            for( ; it.current(); ++it )
            {
                Q3ListViewItem* current = it.current();
                if( MatchItem( current ) && current->isSelectable() && current->isEnabled() )
                {
                    result = current;
                    break;
                }
            }
        }

        if( result == 0 ) // Find from begging if not found or no selection
        {
            for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
            {
                Q3ListViewItem* current = it.current();
                if( MatchItem( current ) && current->isSelectable() && current->isEnabled() )
                {
                    result = current;
                    break;
                }
            }
        }

        if( result != 0 )
        {
            setSelected( result, true );
            setCurrentItem( result );
            ensureItemVisible( result );
        }
    }

    void LockDragAndDrop( bool lock )
    {
        dndLocked_ = lock;
    }

private:
    bool MatchItem( Q3ListViewItem* item )
    {
        if( !item )
            return false;

        QString text = item->text( 0 );
        ValuedListItem* valuedItem = static_cast< ValuedListItem* >( item );
        if( valuedItem->IsA< kernel::Entity_ABC >() )
            if( kernel::Entity_ABC* entity = valuedItem->GetValueNoCheck< kernel::Entity_ABC >() )
            {
                text += " " + QString::number( static_cast< unsigned int >( entity->GetId() ) );
            }
        return text.contains( searchedText_, Qt::CaseInsensitive ) != 0;
    }

public:
    void ApplyFilter( boost::function< bool ( ValuedListItem* ) > func )
    {
        for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
        {
            ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
            item->setVisible( HasAnyChildVisible( item, func ) );
        }
    }

protected:
    bool IsDragAndDropLocked()
    {
        return dndLocked_;
    }

private:
    bool HasAnyChildVisible( ValuedListItem* item, boost::function< bool ( ValuedListItem* ) > func )
    {
        if( item )
        {
            bool isVisible = func( item );
            ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
            if( child )
                for( Q3ListViewItemIterator it = child; it.current(); ++it )
                    if( static_cast< ValuedListItem* >( it.current()->parent() ) == item )
                        isVisible = isVisible || HasAnyChildVisible( static_cast< ValuedListItem* >( it.current() ), func );
            return isVisible;
        }
        return false;
    }

private:
    QString searchedText_;
    bool    creationBlocked_;
    bool    contextMenuBlocked_;
    bool    dndLocked_;
};

// =============================================================================
/** @class  ListView
    @brief  ListView
*/
// Created: AGE 2006-02-20
// =============================================================================
template< typename ConcreteList >
class ListView : public RichListView
               , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ListView( QWidget* parent, ConcreteList& list, ItemFactory_ABC& factory, const char* name = 0 )
                 : RichListView( parent, name )
                 , list_      ( list )
                 , factory_   ( factory )
                 , toSkip_    ( 0 )
                 , comparator_( 0 )
             {
                 setDefaultRenameAction( Q3ListView::Accept );
                 setBackgroundColor( Qt::white );
                 setAcceptDrops( true );
             }
    virtual ~ListView() {}
    //@}

    //! @name Operations
    //@{
    template< typename Element, typename Parent >
    ValuedListItem* Display( tools::Iterator< const Element& > it, Parent* parent, ValuedListItem* currentItem = 0 )
    {
        if( ! parent )
            throw std::runtime_error( "Missing parent !" );
        ValuedListItem* previousItem = currentItem;
        while( it.HasMoreElements() )
        {
            const Element& element = it.NextElement();
            if( ! currentItem  )
                currentItem = CreateItem( parent, previousItem );
            currentItem->SetValue( &element );
            previousItem = currentItem;
            currentItem = (ValuedListItem*)( currentItem->nextSibling() );
            list_.Display( element, previousItem );
        }
        return currentItem ? currentItem : CreateItem( parent, previousItem );
    };
    template< typename Iterator, typename Parent >
    ValuedListItem* Display( Iterator from, const Iterator& to, Parent* parent, ValuedListItem* currentItem = 0 )
    {
        if( ! parent )
            throw std::runtime_error( "Missing parent !" );
        ValuedListItem* previousItem = currentItem;
        while( from != to )
        {
            if( ! currentItem  )
                currentItem = CreateItem( parent, previousItem );
            previousItem = currentItem;
            currentItem = (ValuedListItem*)( currentItem->nextSibling() );
            list_.Display( *from, previousItem );
            ++from;
        }
        return currentItem ? currentItem : CreateItem( parent, previousItem );
    };

    void RemoveItem( ValuedListItem* item )
    {
        if( item->nextSibling() )
            delete item;
        else
            toSkip_ = item;
    }

    void DeleteTail( ValuedListItem* item )
    {
        while( item )
        {
            ValuedListItem* next = (ValuedListItem*) item->nextSibling();
            delete item;
            item = next;
        }
    };

    template< typename Parent >
    ValuedListItem* CreateItem( Parent* parent, ValuedListItem* previousItem )
    {
        if( IsCreationBlocked() )
            return 0;
        if( previousItem && previousItem == toSkip_ ) {
            toSkip_ = 0;
            return previousItem;
        }
        return factory_.CreateItem( parent, previousItem, comparator_ );
    }

    template< typename Parent >
    ValuedListItem* CreateItem( Parent* parent )
    {
        if( IsCreationBlocked() )
            return 0;
        return factory_.CreateItem( parent, comparator_ );
    }

    void SetComparator( ValuedListItem::Comparator comparator )
    {
        comparator_ = comparator;
    }
    //@}

private:
    //! @name Member data
    //@{
    ConcreteList& list_;
    ItemFactory_ABC& factory_;
    ValuedListItem* toSkip_;
    ValuedListItem::Comparator comparator_;
    //@}
};

}

#endif // __ListView_h_
