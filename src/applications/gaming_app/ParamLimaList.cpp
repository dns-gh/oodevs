// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLimaList.h"
#include "moc_ParamLimaList.cpp"
#include "gaming/Lima.h"
#include "gaming/Tools.h"
#include "clients_gui/ValuedListItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( QWidget* pParent, ASN1T_LimasOrder& limas, const QString& label, const QString& menu )
    : QListView  ( pParent )
    , result_    ( limas )
    , menu_      ( menu )
    , pPopupMenu_( new QPopupMenu( this ) )
    , potential_ ( 0 )
{
    result_.n = 0;
    result_.elem = 0;

    addColumn( label );
    addColumn( tr( "Function" ) );
    setResizeMode( QListView::LastColumn );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool ParamLimaList::CheckValidity()
{
    if( !IsOptional() && childCount() == 0 )
    {
        header()->setPaletteForegroundColor( Qt::red );
        QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::Commit
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::Commit()
{
    if( ! childCount() )
        return;
    
    Clean();
    result_.n = childCount();
    result_.elem = new ASN1T_LimaOrder[result_.n];

    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    unsigned int i = 0;
    while( item )
    {
        QStringList functions = QStringList::split( ',', item->text( 1 ) );
        result_.elem[i].fonctions.n = functions.count();
        if( result_.elem[i].fonctions.n > 0 )
        {
            result_.elem[i].fonctions.elem = new ASN1T_EnumTypeLima[result_.elem[i].fonctions.n];
            for( unsigned int j = 0; j < result_.elem[i].fonctions.n; ++j )
                result_.elem[i].fonctions.elem[j] = (ASN1T_EnumTypeLima)tools::FromString( functions[j] );
        }
        const Lima* lima = item->GetValue< const Lima >();
        lima->CopyTo( result_.elem[i].lima );
        item = (ValuedListItem*)( item->nextSibling() );
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::OnRequestPopup
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Clear list" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::OnDeleteSelectedItem
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::OnDeleteSelectedItem()
{
    delete currentItem();
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::OnClearList
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::OnClearList()
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::TurnHeaderBlack
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::TurnHeaderBlack()
{
    header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::MenuItemValidated
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::MenuItemValidated( int function )
{
    if( potential_ )
    {
        gui::ValuedListItem* item = gui::FindItem( potential_, firstChild() );
        if( !item )
            item = new gui::ValuedListItem( this );
        QStringList functions = QStringList::split( ',', item->text( 1 ) );
        if( !functions.contains( tools::ToString( (E_FuncLimaType)function ) ) )
            functions.append( tools::ToString( (E_FuncLimaType)function ) );
        item->Set( potential_, potential_->GetName(), functions.join( "," ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::NotifyContextMenu
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu )
{
    if( !entity.IsLimit() )
    {
        potential_ = static_cast< const Lima* >( &entity );
        QPopupMenu* limaMenu = new QPopupMenu( menu );
        for( int n = 0; n < eLimaFuncNbr; ++n )
        {
            int nId = limaMenu->insertItem( tools::ToString( (E_FuncLimaType)n ), this, SLOT( MenuItemValidated( int ) ) ); 
            limaMenu->setItemParameter( nId, n );
        }
        menu.InsertItem( "Parameter", menu_.c_str(), limaMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::NotifyDeleted
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::NotifyDeleted( const Lima& entity )
{
    delete gui::FindItem( &entity, firstChild() );
    if( &entity == potential_ )
        potential_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::Clean
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::Clean()
{
    for( unsigned int i = 0; i < result_.n; ++i )
        delete[] result_.elem[i].fonctions.elem;
    delete[] result_.elem;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::Draw
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void ParamLimaList::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        if( const Lima* lima = item->GetValue< const Lima >() )
        {
            glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
                glColor4f( 1, 0, 0, 0.5f );
                lima->Interface().Apply( &kernel::Drawable_ABC::Draw, point, viewport, tools );
            glPopAttrib();
        }
        item = (ValuedListItem*)( item->nextSibling() );
    }
}
