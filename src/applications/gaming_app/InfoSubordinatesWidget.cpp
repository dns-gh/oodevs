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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_gui/SymbolIcons.h"

namespace
{
    class ValuedIconViewItem : public QIconViewItem
    {
    public:
        ValuedIconViewItem( QIconView* parent, const QString& text, const QPixmap& icon, const kernel::Entity_ABC& entity )
            : QIconViewItem( parent, text, icon )
            , entity_( &entity )
        {}
        virtual ~ValuedIconViewItem() {}

        const kernel::Entity_ABC* entity_;
    };
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget constructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::InfoSubordinatesWidget( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons )
    : QIconView( parent )
    , controllers_( controllers )
    , icons_( icons )
    , selected_( controllers )
{
    setGridX( 60 );
    setGridY( 60 );
    setFixedWidth( 330 );
    setWordWrapIconText( false );
    setSorting( true );
    setItemsMovable( false );
    connect( this, SIGNAL( doubleClicked( QIconViewItem* ) ), SLOT( OpenItem( QIconViewItem* ) ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget destructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::~InfoSubordinatesWidget()
{
    controllers_.Remove( *this );
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
            kernel::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
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
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        const std::string symbolName = hierarchies->GetSymbol();
        const std::string levelName  = hierarchies->GetLevel();
        if( ! symbolName.empty() || ! levelName.empty() )
            new ValuedIconViewItem( this, entity.GetName(), icons_.GetSymbol( symbolName, levelName ), entity );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::OpenItem
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::OpenItem( QIconViewItem* item )
{
    if( ValuedIconViewItem* value = static_cast< ValuedIconViewItem* >( item ) )
        controllers_.actions_.Select( *value->entity_ );
}
