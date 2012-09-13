// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TacticalTreeView.h"
#include "clients_gui/resources.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"

using namespace gui;

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
            return pAttributes->IsCommandPost();
        return false;
    }

    bool IsEngaged( const kernel::Entity_ABC& entity )
    {
        if( const kernel::AutomatDecisions_ABC* decisions = entity.Retrieve< kernel::AutomatDecisions_ABC >() )
            return decisions->IsEmbraye();
        return false;
    }

    class TacticalItemDelegate : public QItemDelegate
    {
    public:
        TacticalItemDelegate( const StandardModel& dataModel, QObject *parent = 0 )
            : QItemDelegate( parent )
            , dataModel_( dataModel )
            , commandPost_( MAKE_PIXMAP( commandpost ) )
            , lock_( MAKE_PIXMAP( lock ) )
        {
            // NOTHING
        }

        ~TacticalItemDelegate()
        {
            // NOTHING
        }

        virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
        {
            QItemDelegate::paint( painter, option, index );
            if( index.column() == 1 )
            {
                QModelIndex item = dataModel_.GetMainModelIndex( index );
                kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( item );
                if( entity )
                {
                    if( IsCommandPost( *entity ) )
                        DrawPixmap( painter, option.rect, commandPost_ );
                    else if( IsEngaged( *entity ) )
                            DrawPixmap( painter, option.rect, lock_ );
                 }
            }
        }

    private:
        void DrawPixmap( QPainter* painter, const QRect& rect, const QPixmap& pixmap ) const
        {
            painter->save();
            QPoint p = rect.topRight();
            p.rx() -= pixmap.width() + 5;
            p.ry() += ( rect.height() - pixmap.height() ) / 2;
            painter->drawPixmap( p, pixmap );
            painter->restore();
        }

    private:
        const StandardModel& dataModel_;
        QPixmap commandPost_;
        QPixmap lock_;
    };
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : HierarchyTreeView< kernel::TacticalHierarchies >( controllers, profile, modelObserver, symbols, parent )
{
    dataModel_.setColumnCount( 2 );
    header()->setStretchLastSection( false );
    header()->setResizeMode( 0, QHeaderView::Stretch );
    header()->setResizeMode( 1, QHeaderView::Fixed );
    header()->resizeSection( 1, 24 );
    setItemDelegate( new TacticalItemDelegate( dataModel_, this ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::LessThan
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
bool TacticalTreeView::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( !entity1 || !entity2 )
        return false;
    valid = true;
    if( IsCommandPost( *entity1 ) )
        return false;
    if( IsCommandPost( *entity2 ) )
        return true;
    return entity2->GetId() < entity1->GetId();
}
