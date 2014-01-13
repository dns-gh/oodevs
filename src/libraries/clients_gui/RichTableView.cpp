// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTableView.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichTableView constructor
// Created: LGY 2014-01-09
// -----------------------------------------------------------------------------
RichTableView::RichTableView( const QString& objectName, QWidget* parent )
    : gui::RichWidget< QTableView >( objectName, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTableView destructor
// Created: LGY 2014-01-09
// -----------------------------------------------------------------------------
RichTableView::~RichTableView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTableView::ResizeColumnsToContents
// Created: LGY 2014-01-09
// -----------------------------------------------------------------------------
void RichTableView::ResizeColumnsToContents()
{
    resizeColumnsToContents();
}
