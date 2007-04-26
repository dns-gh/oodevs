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
#include "gaming/ActionParameterLimaList.h"
#include "gaming/ActionParameterLima.h"
#include "gaming/Action_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Lines.h"
#include "game_asn/Asn.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLimaList constructor
// Created: SBO 2006-11-14
// ----------------------------------------------------------------------------
ParamLimaList::ParamLimaList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter )
    : QObject    ( parent )
    , Param_ABC  ( parameter.GetName() )
    , converter_ ( converter )
    , parameter_ ( parameter )
    , list_      ( 0 )
    , potential_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList destructor
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
ParamLimaList::~ParamLimaList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CheckValidity
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
bool ParamLimaList::CheckValidity()
{
    if( !parameter_.IsOptional() && list_->childCount() == 0 )
    {
        list_->header()->setPaletteForegroundColor( Qt::red );
        QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamLimaList::BuildInterface( QWidget* parent )
{
    list_ = new QListView( parent );
    pPopupMenu_ = new QPopupMenu( list_ );
    
    list_->addColumn( GetName() );
    list_->addColumn( tr( "Function" ) );
    list_->setResizeMode( QListView::LastColumn );
    connect( list_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLimaList::CommitTo( ASN1T_OrderContext& asn ) const
{
    if( !list_ )
        InterfaceNotInitialized();
    asn.limas.n = 0;
    asn.limas.elem = 0;
    if( ! list_->childCount() )
        return;

    ASN1T_LimasOrder& list = asn.limas;
    list.n = list_->childCount();
    list.elem = new ASN1T_LimaOrder[list.n];

    ValuedListItem* item = (ValuedListItem*)( list_->firstChild() );
    unsigned int i = 0;
    while( item )
    {
        QStringList functions = QStringList::split( ',', item->text( 1 ) );
        list.elem[i].fonctions.n = functions.count();
        if( list.elem[i].fonctions.n > 0 )
        {
            list.elem[i].fonctions.elem = new ASN1T_EnumTypeLima[list.elem[i].fonctions.n];
            for( unsigned int j = 0; j < list.elem[i].fonctions.n; ++j )
                list.elem[i].fonctions.elem[j] = (ASN1T_EnumTypeLima)tools::FromString( functions[j] );
        }
        const Lima* lima = item->GetValue< const Lima >();
        lima->CopyTo( list.elem[i].lima );
        item = (ValuedListItem*)( item->nextSibling() );
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamLimaList::Clean( ASN1T_OrderContext& asn ) const
{
    for( unsigned int i = 0; i < asn.limas.n; ++i )
        delete[] asn.limas.elem[i].fonctions.elem;
    delete[] asn.limas.elem;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::CommitTo
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ParamLimaList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameterLimaList > param( new ActionParameterLimaList( parameter_ ) );
    ValuedListItem* item = (ValuedListItem*)( list_->firstChild() );
    QStringList functions;
    unsigned int i = 0;
    while( item )
    {
        const Lima* lima = item->GetValue< const Lima >();
        if( !lima )
            throw std::runtime_error( "Invalid lima" );
        kernel::Lines lines;
        lima->CopyTo( lines );
        param->AddParameter( *new ActionParameterLima( tr( "Lima %1" ).arg( ++i ), converter_, lines, item->text( 1 ) ) );
        item = (ValuedListItem*)( item->nextSibling() );
    }
    action.AddParameter( *param.release() );
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
    delete list_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::OnClearList
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::OnClearList()
{
    list_->clear();
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::TurnHeaderBlack
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::TurnHeaderBlack()
{
    list_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::MenuItemValidated
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::MenuItemValidated( int function )
{
    if( potential_ )
    {
        gui::ValuedListItem* item = gui::FindItem( potential_, list_->firstChild() );
        if( !item )
            item = new gui::ValuedListItem( list_ );
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
        menu.InsertItem( "Parameter", tr( "Add %1" ).arg( GetName() ), limaMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::NotifyDeleted
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void ParamLimaList::NotifyDeleted( const Lima& entity )
{
    delete gui::FindItem( &entity, list_->firstChild() );
    if( &entity == potential_ )
        potential_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ParamLimaList::Draw
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void ParamLimaList::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    ValuedListItem* item = (ValuedListItem*)( list_->firstChild() );
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
