// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NatureEditionWidget.h"
#include "moc_NatureEditionWidget.cpp"
#include "NatureEditionCategory.h"

#include "clients_kernel/SymbolRule.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget constructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionWidget::NatureEditionWidget( QWidget* parent, const std::string& symbolFile )
    : QVBox( parent )
    , root_( 0 )
{
    xml::xifstream xis( symbolFile );
    xis >> xml::start( "app6" )
        >> xml::start( "symbols" )
        >> xml::start( "choice" );
    root_ = new SymbolRule( xis );
    rootWidget_ = new NatureEditionCategory( this, root_ );

    connect( rootWidget_, SIGNAL( NatureChanged( const QString& ) ), this, SIGNAL( textChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionWidget::~NatureEditionWidget()
{
    delete rootWidget_;
    delete root_;
}

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget::text
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
QString NatureEditionWidget::text()
{
    return rootWidget_->GetNature();
}

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget::setText
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionWidget::setText( const QString& nature )
{
    rootWidget_->SetNature( nature );
}
