// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "XmliaReport.h"

#include <qdatetime.h>
// -----------------------------------------------------------------------------
// Name: XmliaReport constructor
// Created: RPD 2009-06-11
// -----------------------------------------------------------------------------
XmliaReport::XmliaReport( QListView* parent, const QString& report )
    : QListViewItem( parent )
{
    report_.setContent( report );
    setMultiLinesEnabled( true );
    setText( 0, QTime::currentTime().toString() );
    QString temp = report_.toString();
    temp.truncate(30);
    temp.append( "..." );
    setText( 1, temp );
}

// -----------------------------------------------------------------------------
// Name: XmliaReport destructor
// Created: RPD 2009-06-11
// -----------------------------------------------------------------------------
XmliaReport::~XmliaReport()
{

}

// -----------------------------------------------------------------------------
// Name: XmliaReport::activate
// Created: RPD 2009-06-11
// -----------------------------------------------------------------------------
void XmliaReport::activate()
{
  QDialog* view = new QDialog( );
  view->setMinimumSize(600, 600);
  view->setMaximumSize(600, 600);

  QTextEdit* text = new QTextEdit( view);
  text->setText( report_.toString() );
  text->setMinimumSize( 600, 600);
  view->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
  view->setPaletteBackgroundColor( QColor( 255, 255, 225 ) );
  view->setCaption( "Report" );
  view->show();
}
