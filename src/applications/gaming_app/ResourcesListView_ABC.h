// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ResourcesListView_ABC_h_
#define __ResourcesListView_ABC_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Availability.h"
#include "gaming/LogisticHelpers.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <QtGui/qtreeview.h>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Displayer_ABC;
}

// =============================================================================
/** @class  ResourcesListView_ABC
    @brief  ResourcesListView_ABC
*/
// Created: SBO 2007-02-16
// =============================================================================
template< typename Extension >
class ResourcesListView_ABC : public QTreeView
                            , public tools::Observer_ABC
                            , public tools::ElementObserver_ABC< Extension >
                            , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourcesListView_ABC( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ResourcesListView_ABC();
    //@}

protected:
    //! @name Operations
    //@{
    bool ShouldUpdate( const Extension& a ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void UpdateSelected( const kernel::Entity_ABC* entity );
    void ResizeModelOnNewContent( int wantedSize );
    //@}

    //! @name Helpers
    //@{
    virtual void polish();
    virtual void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Extension& a ) = 0;
    bool HasRetrieveForLogistic( const kernel::Entity_ABC& entity );
    bool HasRetrieveForLogistic( const kernel::Entity_ABC& entity, const Extension& a );
    void DisplayModelWithAvailabilities();
    void DisplaySelectionAvailabilities();
    void AddAvailability( const kernel::Entity_ABC& entity );
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const Extension& ) const { return nullptr; }
    //@}

    //! @name Member data
    //@{
private:
    kernel::Controllers& controllers_;

protected:
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QStandardItemModel model_;
    std::map< std::string, kernel::Availability > availabilities_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
ResourcesListView_ABC< Extension >::ResourcesListView_ABC( QWidget* parent, kernel::Controllers& controllers )
    : QTreeView( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    header()->setResizeMode( QHeaderView::ResizeToContents );
    setModel( &model_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
ResourcesListView_ABC<  Extension >::~ResourcesListView_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::ShouldUpdate
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
bool ResourcesListView_ABC< Extension >::ShouldUpdate( const Extension& a ) const
{
    return isVisible() && selected_ && selected_->Retrieve< Extension >() == &a;
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::polish
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::polish()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::showEvent
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
    QTreeView::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::NotifySelected
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( const Extension* extension = selected_ ? selected_->Retrieve< Extension >() : 0 )
    {
        show();
        NotifyUpdated( *extension );
    }
    else
        hide();
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::UpdateSelected
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::UpdateSelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    if( const Extension* extension = selected_->Retrieve< Extension >() )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::ResizeModelOnContent
// Created: NPT 2012-10-23
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::ResizeModelOnNewContent( int wantedSize )
{
    int modelSize = model_.rowCount();
    if( modelSize > wantedSize )
        model_.removeRows( wantedSize, modelSize - wantedSize );
    else if( modelSize < wantedSize )
        for( int i = 0; i < wantedSize - modelSize; ++i )
        {
            QList< QStandardItem *> list;
            for( int j = 0; j < model_.columnCount(); ++j )
                list.append( new QStandardItem() );
            model_.appendRow( list );
        }
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::HasRetrieveForLogistic
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
template< typename Extension >
bool ResourcesListView_ABC< Extension >::HasRetrieveForLogistic( const kernel::Entity_ABC& entity )
{
    return logistic_helpers::HasRetrieveEntityAndSubordinatesUpToBaseLog< Extension >( entity );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::HasRetrieveForLogistic
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
template< typename Extension >
bool ResourcesListView_ABC< Extension >::HasRetrieveForLogistic( const kernel::Entity_ABC& entity, const Extension& a )
{
    return logistic_helpers::HasRetrieveEntityAndSubordinatesUpToBaseLog( entity, &a );
}

// -----------------------------------------------------------------------------
// Name: HasRetrieveForLogistic
// Created: MMC 2013-01-23
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::DisplayModelWithAvailabilities()
{
    ResizeModelOnNewContent( static_cast< int >( availabilities_.size() ) );
    unsigned int index = 0;
    for( auto it = availabilities_.begin(); it != availabilities_.end(); ++it )
    {
        model_.item( index, 0 )->setText( QString( it->first.c_str() ) );
        model_.item( index, 1 )->setText( QString::number( it->second.total_ ) );
        model_.item( index, 2 )->setText( QString::number( it->second.available_ ) );
        model_.item( index, 3 )->setText( QString::number( it->second.atWork_ ) );
        model_.item( index, 4 )->setText( QString::number( it->second.atRest_ ) );
        model_.item( index, 5 )->setText( QString::number( it->second.lent_ ) );
        ++index;
    }
}

// -----------------------------------------------------------------------------
// Name: DisplaySelectionAvailabilities
// Created: MMC 2013-02-08
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::AddAvailability( const kernel::Entity_ABC& entity )
{
    if( const Extension* pState = entity.Retrieve< Extension >() )
    {
        const std::vector< kernel::Availability >* curAvailabilies = GetAvailabilities( *pState );
        if( curAvailabilies )
            for( auto it = curAvailabilies->begin(); it != curAvailabilies->end(); ++it )
                availabilities_[ it->type_->GetName() ] += *it;
    }
}

// -----------------------------------------------------------------------------
// Name: DisplaySelectionAvailabilities
// Created: MMC 2013-02-08
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::DisplaySelectionAvailabilities()
{
    if( !selected_ )
        return;
    availabilities_.clear();
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( *selected_, boost::bind( &ResourcesListView_ABC< Extension >::AddAvailability, this, _1 ) );
    DisplayModelWithAvailabilities();
}

#endif // __ResourcesListView_ABC_h_
