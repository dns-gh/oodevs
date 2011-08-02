// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NatureSelectionWidget.h"
#include "moc_NatureSelectionWidget.cpp"
#include "clients_kernel/SymbolRule.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
NatureSelectionWidget::NatureSelectionWidget( QWidget* parent, const tools::Loader_ABC& loader )
    : Q3ListView( parent )
    , category_( 0 )
    , current_ ( 0 )
{
    addColumn( "" );
    header()->hide();
    setRootIsDecorated( true );
    setResizeMode( Q3ListView::LastColumn );
    loader.LoadPhysicalFile( "symbols", boost::bind( &NatureSelectionWidget::LoadSymbols, this, _1 ) );
    connect( this, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
NatureSelectionWidget::~NatureSelectionWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::LoadSymbols
// Created: LDC 2011-06-29
// -----------------------------------------------------------------------------
void NatureSelectionWidget::LoadSymbols( xml::xistream& xis )
{
    xis >> xml::start( "app6" )
            >> xml::start( "symbols" )
                >> xml::start( "choice" );
    kernel::SymbolRule root( xis );
    root.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::StartCategory
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::StartCategory( const std::string& /*title*/ )
{
    category_ = current_;
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::EndCategory
// Created: LDC 2010-10-04
// -----------------------------------------------------------------------------
void NatureSelectionWidget::EndCategory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::AddChoice
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::AddChoice( kernel::SymbolRule* rule, const std::string& name, const std::string& /*value*/ )
{
    if( !category_ )
        current_ = new Q3ListViewItem( this );
    else
        current_ = new Q3ListViewItem( category_ );
    current_->setText( 0, name.c_str() );
    current_->setDragEnabled( true );
    if( rule )
    {
        Q3ListViewItem* category = category_;
        rule->Accept( *this );
        category_ = category;
    }
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::OnSelectionChanged
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::OnSelectionChanged( Q3ListViewItem* item )
{
    QStringList list;
    while( item )
    {
        list.push_front( item->text( 0 ) );
        item = item->parent();
    }
    const QString nature = list.join( "/" );
    emit NatureSelected( nature );
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::startDrag
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void NatureSelectionWidget::startDrag()
{
    emit StartDrag();
}
