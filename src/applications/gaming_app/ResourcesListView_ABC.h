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
#include "clients_kernel/EquipmentType.h"
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

Q_DECLARE_METATYPE( const kernel::Availability* );

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
             ResourcesListView_ABC( QWidget* parent,
                                    kernel::Controllers& controllers,
                                    bool listenSelectionChanged = true );
    virtual ~ResourcesListView_ABC();
    //@}

    // If a filter is set, only accepted Availability will be displayed
    void SetFilter( const std::function< bool( const kernel::Availability& ) >& filter );

protected:
    //! @name Operations
    //@{
    bool ShouldUpdate( const Extension& a ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void SelectEntity( const kernel::Entity_ABC* entity );
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
    void AddItem( int row, int column, const QString& text, const kernel::Availability& type );
    //@}

    //! @name Member data
    //@{
private:
    kernel::Controllers& controllers_;
    std::function< bool( const kernel::Availability& ) > filter_;
    bool listenSelectionChanged_;

protected:
    QStandardItemModel model_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::vector< kernel::Availability > availabilities_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
ResourcesListView_ABC< Extension >::ResourcesListView_ABC( QWidget* parent,
                                                           kernel::Controllers& controllers,
                                                           bool listenSelectionChanged /* = true */ )
    : QTreeView( parent )
    , controllers_( controllers )
    , selected_( controllers )
    , listenSelectionChanged_( listenSelectionChanged )
{
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    header()->setResizeMode( QHeaderView::ResizeToContents );
    header()->setStretchLastSection( false );
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
    SelectEntity( selected_ );
    QTreeView::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::NotifySelected
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( !listenSelectionChanged_ )
        return;
    SelectEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::SelectEntity
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::SelectEntity( const kernel::Entity_ABC* entity )
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
// Name: ResourcesListView_ABC::AddItem
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::AddItem( int row, int column, const QString& text, const kernel::Availability& availability )
{
    QStandardItem* item = model_.item( row, column );
    item->setText( text );
    item->setData( QVariant::fromValue( &availability ), Qt::UserRole );
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
        if( it->type_ == 0 || it->entity_ == 0 )
            throw MASA_EXCEPTION( "Missing equipment type or entity in Availability" );
        AddItem( index, 0, QString::fromStdString( it->type_->GetName() ), *it );
        AddItem( index, 1, it->entity_->GetName(), *it );
        AddItem( index, 2, QString::number( it->total_ ), *it );
        AddItem( index, 3, QString::number( it->available_ ), *it );
        AddItem( index, 4, QString::number( it->atWork_ ), *it );
        AddItem( index, 5, QString::number( it->atRest_ ), *it );
        AddItem( index, 6, QString::number( it->lent_ ), *it );
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
                if( !filter_ || filter_( *it ) )
                {
                    auto availability = std::find_if( availabilities_.begin(), availabilities_.end(),
                        [&]( kernel::Availability& value )->bool {
                            return value.entity_->GetId() == (*it).entity_->GetId() &&
                                   value.type_->GetId() == (*it).type_->GetId(); } );
                    if( availability != availabilities_.end() )
                        *availability += *it;
                    else
                        availabilities_.push_back( *it );
                }
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
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( *selected_, [&] ( const kernel::Entity_ABC& entity )
    {
        AddAvailability( entity );
    } );
    DisplayModelWithAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::SetFilter
// Created: ABR 2014-02-05
// -----------------------------------------------------------------------------
template< typename Extension >
void ResourcesListView_ABC< Extension >::SetFilter( const std::function< bool( const kernel::Availability& ) >& filter )
{
    filter_ = filter;
}

#endif // __ResourcesListView_ABC_h_
