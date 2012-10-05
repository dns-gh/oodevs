// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TemplateListView.h"
#include "moc_TemplateListView.cpp"
#include "preparation/HierarchyTemplate.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/LongNameHelper.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <tools/XmlCrc32Signature.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: TemplateListView constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplateListView::TemplateListView( QWidget* parent, kernel::Controllers& controllers, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, ColorController& colorController )
    : gui::RichTreeView( parent, &controllers )
    , agents_         ( agents )
    , formations_     ( formations )
    , types_          ( types )
    , colorController_( colorController )
{
    EnableDragAndDrop( true );
    setHeaderHidden( true );
    setEditTriggers( SelectedClicked | EditKeyPressed );
    connect( &dataModel_, SIGNAL( DataChanged( const QModelIndex&, const QVariant& ) ), SLOT( OnRename( const QModelIndex&, const QVariant& ) ) );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView destructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplateListView::~TemplateListView()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::CreateTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::CreateTemplate( const kernel::Entity_ABC& entity )
{
    HierarchyTemplate* pTemplate = new HierarchyTemplate( agents_, formations_, entity, true, colorController_ );
    templates_.push_back( pTemplate );
    std::string longName = gui::longname::GetEntityLongName( entity );
    if( longName.empty() )
        CreateItem( *pTemplate );
    else
        CreateItem( *pTemplate, QString::fromStdString( longName ) );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::MimeTypes
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
QStringList TemplateListView::MimeTypes() const
{
    return QStringList( typeid( HierarchyTemplate ).name() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::contextMenuEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void TemplateListView::contextMenuEvent( QContextMenuEvent* event )
{
    if( event && indexAt( event->pos() ) == selectionModel()->currentIndex() )
    {
        QMenu* menu = new QMenu( this  );
        menu->addAction( tr( "Rename" ), this, SLOT( OnRename() ) );
        menu->popup( event->globalPos() );
    }
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::OnRename
// Created: LGY 2012-03-06
// -----------------------------------------------------------------------------
void TemplateListView::OnRename()
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( index.isValid() )
        edit( index );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::OnRename
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void TemplateListView::OnRename( const QModelIndex& index, const QVariant& variant )
{
    if( index.isValid() && variant.type() == QVariant::String )
        if( HierarchyTemplate* pTemplate = dataModel_.GetDataFromIndex< HierarchyTemplate >( index ) )
            pTemplate->Rename( variant.toString() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::LoadTemplates
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::LoadTemplates( const std::string& filename )
{
    try
    {
        Clear();
        if( !filename.empty() && bfs::exists( bfs::path( filename ) ) )
        {
            xml::xifstream input( filename );
            input >> xml::start( "templates" )
                  >> xml::list( "template", *this, &TemplateListView::ReadTemplate );
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::SaveTemplates
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::SaveTemplates( const std::string& filename ) const
{
    {
        xml::xofstream output( filename );
        output << xml::start( "templates" );
        for( CIT_Templates it = templates_.begin(); it != templates_.end(); ++it )
            (*it)->Serialize( output );
        output << xml::end;
    }
    tools::WriteXmlCrc32Signature( filename );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::ReadTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::ReadTemplate( xml::xistream& input )
{
    templates_.push_back( new HierarchyTemplate( agents_, formations_, types_, input, colorController_ ) );
    CreateItem( *templates_.back() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::CreateItem
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::CreateItem( HierarchyTemplate& t )
{
    CreateItem( t, t.GetName() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::CreateItem
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::CreateItem( HierarchyTemplate& t, const QString& name )
{
    dataModel_.AddRootDataItem( dataModel_.rowCount(), 0, name, "", t, Qt::ItemIsDragEnabled | Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::Clear
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::Clear()
{
    dataModel_.Purge();
    for( CIT_Templates it = templates_.begin(); it != templates_.end(); ++it )
        delete *it;
    templates_.clear();
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::keyPressEvent
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::keyPressEvent( QKeyEvent* evt )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( evt && evt->key() == Qt::Key_Delete && index.isValid() )
    {
        if( HierarchyTemplate* pTemplate = dataModel_.GetDataFromIndex< HierarchyTemplate >( index ) )
        {
            IT_Templates it = std::find( templates_.begin(), templates_.end(), pTemplate );
            if( it != templates_.end() )
            {
                *it = templates_.back();
                templates_.pop_back();
                delete pTemplate;
                
                QStandardItem* item = dataModel_.GetItemFromIndex( index );
                QList< QStandardItem* > rowItems = dataModel_.invisibleRootItem()->takeRow( item->row() );
                for( QList< QStandardItem* >::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
                    delete *it;
            }
        }
    }
    else
        RichTreeView::keyPressEvent( evt );
}
