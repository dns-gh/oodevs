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
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/ValuedDragObject.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TemplateListView constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplateListView::TemplateListView( QWidget* parent, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types )
    : Q3ListView  ( parent )
    , agents_    ( agents )
    , formations_( formations )
    , types_     ( types )
{
    addColumn( tr( "Template" ) );
    setResizeMode( Q3ListView::AllColumns );
    header()->hide();
    setSorting( -1 );

    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
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
    templates_.push_back( new HierarchyTemplate( agents_, formations_, entity ) );
    CreateItem( *templates_.back() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::dragObject
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
Q3DragObject* TemplateListView::dragObject()
{
    ValuedListItem* pItem = static_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;

    const HierarchyTemplate* pTemplate = pItem->GetValue< HierarchyTemplate >();
    return new ValuedDragObject( pTemplate, this );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::OnRename
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::OnRename( Q3ListViewItem* item, int, const QString& newName )
{
    if( ValuedListItem* pItem = static_cast< ValuedListItem* >( item ) )
    {
        HierarchyTemplate* pTemplate = pItem->GetValue< HierarchyTemplate >();
        pTemplate->Rename( newName );
        item->setText( 0, newName );
    }
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
        if( ! filename.empty() && bfs::exists( bfs::path( filename, bfs::native ) ) )
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
    templates_.push_back( new HierarchyTemplate( agents_, formations_, types_, input ) );
    CreateItem( *templates_.back() );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::CreateItem
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::CreateItem( HierarchyTemplate& t )
{
    ValuedListItem* item = new ValuedListItem( this );
    item->Set( &t, t.GetName() );
    item->setDragEnabled( true );
    item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::Clear
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::Clear()
{
    clear();
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
    if( evt && evt->key() == Qt::Key_Delete && selectedItem() )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( selectedItem() );
        HierarchyTemplate* t = item->GetValue< HierarchyTemplate >();
        IT_Templates it = std::find( templates_.begin(), templates_.end(), t );
        if( it != templates_.end() )
        {
            delete *it;
            std::swap( *it, templates_.back() );
            templates_.pop_back();
            delete item;
        }
    }
    else
        Q3ListView::keyPressEvent( evt );
}
