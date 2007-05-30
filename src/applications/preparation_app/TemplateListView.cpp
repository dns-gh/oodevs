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
#include "preparation/HierarchyTemplate.h"
#include "clients_gui/ValuedListItem.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "xeumeuleu/xml.h"

namespace bfs = boost::filesystem;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TemplateListView constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
TemplateListView::TemplateListView( QWidget* parent, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types )
    : QListView  ( parent )
    , agents_    ( agents )
    , formations_( formations )
    , types_     ( types )
{
    addColumn( tr( "Template" ) );
    header()->hide();
    setSorting( -1 );
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
    ValuedListItem* item = new ValuedListItem( this );
    item->SetNamed( *templates_.back() );
    item->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::dragObject
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QDragObject* TemplateListView::dragObject()
{
    ValuedListItem* pItem = static_cast< ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;

    const HierarchyTemplate* pTemplate = pItem->GetValue< const HierarchyTemplate >();
    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)( &pTemplate ), sizeof( const HierarchyTemplate* ) );
    QStoredDrag* data = new QStoredDrag( "csword/HierarchyTemplate", this );
    data->setEncodedData( *pBytes );
    return data;
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
    xml::xofstream output( filename );
    output << xml::start( "templates" );
    for( CIT_Templates it = templates_.begin(); it != templates_.end(); ++it )
        (*it)->Serialize( output );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: TemplateListView::ReadTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void TemplateListView::ReadTemplate( xml::xistream& input )
{
    templates_.push_back( new HierarchyTemplate( agents_, formations_, types_, input ) );
    ValuedListItem* item = new ValuedListItem( this );
    item->SetNamed( *templates_.back() );
    item->setDragEnabled( true );
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
