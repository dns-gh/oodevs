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
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
NatureEditionWidget::NatureEditionWidget( QGridLayout* layout, int row /*= 0*/, int deep /*= -1*/ )
    : QWidget( ( layout && layout->parentWidget() ) ? layout->parentWidget() : 0 )
{
    assert( deep != 0 );
    rootWidget_ = new NatureEditionCategory( layout, row, ( deep < 0 ) ? -1 : deep - 1 );
    connect( rootWidget_, SIGNAL( NatureChanged( const QString& ) ), this, SIGNAL( textChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionWidget::~NatureEditionWidget()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget::SetRootSymbolRule
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void NatureEditionWidget::SetRootSymbolRule( kernel::SymbolRule& root )
{
    rootWidget_->SetRootSymbolRule( root );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionWidget::Clear
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
void NatureEditionWidget::Clear()
{
    rootWidget_->Clear();
}
