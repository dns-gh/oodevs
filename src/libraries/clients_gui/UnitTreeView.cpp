// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitTreeView.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitTreeView constructor
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
UnitTreeView::UnitTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::AgentTypes& types, QWidget* parent, bool selectOnlyAutomats /*= false*/ )
    : RichTreeView( objectName, parent, &controllers )
    , controllers_( controllers )
    , types_( types )
    , selectOnlyAutomats_( selectOnlyAutomats )
{
    dropAction_ = Qt::CopyAction;
    EnableDragAndDrop( true );
    setHeaderHidden( true );
    dataModel_.setColumnCount( 2 );
    header()->setStretchLastSection( false );
    header()->setResizeMode( 0, QHeaderView::Stretch );
    header()->setResizeMode( 1, QHeaderView::Fixed );
    header()->resizeSection( 1, 32 );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView destructor
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
UnitTreeView::~UnitTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::SetSorting
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::SetSorting( const std::string& nature )
{
    sorting_ = nature;
    dataModel_.Purge();
    FillList();
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::MimeTypes
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
QStringList UnitTreeView::MimeTypes() const
{
    QStringList list;
    list << typeid( kernel::AutomatType ).name() << typeid( kernel::AgentType ).name();
    return list;
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::NotifyUpdated
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::NotifyUpdated( const kernel::ModelLoaded& )
{
    FillList();
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::NotifyUpdated
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    bool wasSelectBlocked = selectionModel()->blockSignals( true );
    bool wasBlocked = blockSignals( true );
    Purge();
    blockSignals( wasBlocked );
    selectionModel()->blockSignals( wasSelectBlocked );
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::FillList
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::FillList()
{
     if( sorting_ == "level" )
         FillListBy( &kernel::AgentNature::GetLevel );
     else if( sorting_ == "atlas" )
         FillListBy( &kernel::AgentNature::GetAtlas );
     else if( sorting_ == "nature" )
         FillListByNature();
     else
    {
        auto it = types_.Resolver< kernel::AutomatType >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::AutomatType& type = it.NextElement();
            const int row = dataModel_.rowCount();
            QStandardItem* item = dataModel_.AddRootDataItem( row, 0, type.GetName().c_str(), "", type, Qt::ItemIsDragEnabled );
            dataModel_.AddRootItem( row, 1 );
            FillAutomatComposition( *item, type );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::*function )
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::FillListBy( const std::string&( kernel::AgentNature::*function )() const )
{
    auto it = types_.Resolver< kernel::AgentType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AgentType& type = it.NextElement();
        const QString text = ( type.GetNature().*function )().c_str();
        QStandardItem* parent = dataModel_.FindTextItem( text );
        if( !parent )
            parent = dataModel_.AddRootTextItem( dataModel_.rowCount(), 0, text, "" );
        QStandardItem* item = dataModel_.AddChildDataItem( parent, parent->rowCount(), 0, type.GetLocalizedName().c_str(), "", type, Qt::ItemIsDragEnabled );
        FillAgentComposition( *item, type );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::CreateNaturePath
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
QStandardItem* UnitTreeView::CreateNaturePath( const std::string& path )
{
    const std::string::size_type pos = path.find_last_of( '/' );
    QStandardItem* parent = dataModel_.invisibleRootItem();
    QString text( path.c_str() );
    if( pos != std::string::npos )
    {
        const std::string head = path.substr( 0, pos );
        const std::string tail = path.substr( pos + 1, path.length() - pos - 1 );
        parent = CreateNaturePath( head );
        text = tail.c_str();
    }
    QStandardItem* result = 0;
    for( int i = 0; i < parent->rowCount(); ++i )
    {
        QStandardItem* child = parent->child( i );
        if( child->text() == text )
        {
            result = child;
            break;
        }
    }
    if( !result )
        result = dataModel_.AddChildTextItem( parent, parent->rowCount(), 0, text, "" );
    return result;
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::FillListByNature
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::FillListByNature()
{
    auto it = types_.Resolver< kernel::AgentType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AgentType& type = it.NextElement();
        const std::string& nature = type.GetNature().GetNature();
        QStandardItem* parentItem = CreateNaturePath( nature );
        QStandardItem* item = dataModel_.AddChildDataItem( parentItem, parentItem->rowCount(), 0, type.GetLocalizedName().c_str(), "", type, Qt::ItemIsDragEnabled );
        FillAgentComposition( *item, type );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::FillAutomatComposition
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::FillAutomatComposition( QStandardItem& parent, const kernel::AutomatType& type )
{
    const auto& compositions = type.GetCompositions();
    for( auto composition = compositions.begin(); composition != compositions.end(); ++composition )
    {
        const int row = parent.rowCount();
        const kernel::AgentType& agentType = composition->GetType();
        QStandardItem* item = dataModel_.AddChildDataItem( &parent, row, 0, agentType.GetLocalizedName().c_str(), "", agentType, Qt::ItemIsDragEnabled );
        item->setSelectable( !selectOnlyAutomats_ );
        if( &agentType == type.GetTypePC() )
            item->setIcon( QIcon( MAKE_PIXMAP( commandpost ) ) );
        QString cnt;
        auto min = composition->GetMin();
        auto max = composition->GetMax();
        if( max != min )
            cnt = QString( "%L1..%L2" ).arg( min ).arg( max == std::numeric_limits< unsigned int >::max() ? "*" : locale().toString( max ) );
        else
            cnt = locale().toString( min );
        QStandardItem* item2 = dataModel_.AddChildTextItem( &parent, row, 1, cnt, "" );
        item2->setSelectable( !selectOnlyAutomats_ );
        FillAgentComposition( *item, agentType );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::FillAgentComposition
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void UnitTreeView::FillAgentComposition( QStandardItem& parent, const kernel::AgentType& type )
{
    auto it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AgentComposition& composante = it.NextElement();
        const int row = parent.rowCount();
        QStandardItem* item = dataModel_.AddChildTextItem( &parent, row, 0, composante.GetType().GetName().c_str(), "" );
        item->setSelectable( false );
        QBrush brush = item->foreground();
        brush.setColor( Qt::darkGray );
        item->setForeground( brush );
        QStandardItem* item2 = dataModel_.AddChildTextItem( &parent, row, 1, locale().toString( composante.GetCount() ), "" );
        item2->setForeground( brush );
        item2->setSelectable( false );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::Select
// Created: NPT 2012-11-13
// -----------------------------------------------------------------------------
void UnitTreeView::Select( const kernel::Automat_ABC& element )
{
    SearchAndSelect( element.GetType().GetName().c_str(), Qt::MatchExactly, Qt::DisplayRole );
}

// -----------------------------------------------------------------------------
// Name: UnitTreeView::Select
// Created: NPT 2012-11-13
// -----------------------------------------------------------------------------
void UnitTreeView::Select( const kernel::Agent_ABC& element )
{
    SearchAndSelect( element.GetType().GetLocalizedName().c_str(), Qt::MatchExactly, Qt::DisplayRole );
}
