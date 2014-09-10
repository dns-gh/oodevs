// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSubordinatesWidget.h"
#include "moc_InfoSubordinatesWidget.cpp"
#include "InfoSubordinateItem.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/StandardIconProxyStyle.h"
#include "gaming/Attributes.h"

Q_DECLARE_METATYPE( const kernel::Entity_ABC* );

namespace
{
    class ItemDelegate : public QItemDelegate
    {
    public:
        ItemDelegate( QObject* parent ) : QItemDelegate( parent )
        {
            // NOTHING
        }
        virtual ~ItemDelegate()
        {
            // NOTHING
        }
        virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
        {
            QItemDelegate::paint( painter, option, index );
            if( const kernel::Entity_ABC* entity = index.model()->data( index, Qt::UserRole + 1 ).value< const kernel::Entity_ABC* >() )
                if( const Attributes* attributes = entity->Retrieve< Attributes >() )
                    DrawLife( painter, attributes->nRawOpState_, option.rect );
        }
    private:
        void DrawLife( QPainter* p, int life, const QRect& rect ) const
        {
            int width = std::min( rect.width(), 50 );
            int posx = std::max( 0, ( rect.width() - 50 ) / 2 );
            const QRect lifeRect( rect.left() + posx, rect.bottom() - 5, width, 5 );
            QColor color;
            color.setHsv( life, 255, 255 );
            p->fillRect( lifeRect, QColor( 255, 100, 100 ) );
            p->fillRect( lifeRect.left(), lifeRect.top(), static_cast< int >( 0.01f * lifeRect.width() * life ), lifeRect.height(), color );
            p->drawRect( lifeRect );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget constructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::InfoSubordinatesWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const gui::EntitySymbols& icons )
    : QListWidget( parent )
    , controllers_( controllers )
    , profile_( profile )
    , icons_( icons )
    , selected_( controllers )
{
    setViewMode( QListView::IconMode );
    setFixedWidth( 345 );
    setWordWrap( true );
    setItemDelegate( new ItemDelegate( this ) );
    setStyle( new gui::StandardIconProxyStyle() );
    setResizeMode( QListView::Adjust );

    connect( this, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), SLOT( OpenItem( QListWidgetItem* ) ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget destructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::~InfoSubordinatesWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::NotifySelected
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        clear();
        hide();
        if( !selected_ )
            return;
        if( const kernel::TacticalHierarchies* hierarchies = selected_->Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                if( !isShown() )
                    show();
                const kernel::Entity_ABC& element = it.NextElement();
                AddSubordinate( element );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::AddSubordinate
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::AddSubordinate( const kernel::Entity_ABC& entity )
{
    if( profile_.IsVisible( entity ) )
        new InfoSubordinateItem( this, controllers_, icons_, entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::OpenItem
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::OpenItem( QListWidgetItem* item )
{
    if( InfoSubordinateItem* value = static_cast< InfoSubordinateItem* >( item ) )
        controllers_.actions_.SetSelected( value->GetEntity(), false );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::NotifyDeleted
// Created: SBO 2007-02-23
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    for( int row = 0; row < count(); ++row )
        if( &static_cast< InfoSubordinateItem* >( item( row ) )->GetEntity() == &entity )
        {
            delete takeItem( row );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::NotifyUpdated
// Created: ABR 2014-09-10
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    for( int row = 0; row < count(); ++row )
        if( &static_cast< InfoSubordinateItem* >( item( row ) )->GetEntity() == &entity )
        {
            auto selected = selected_;
            selected_ = 0;
            NotifySelected( selected );
        }
}
