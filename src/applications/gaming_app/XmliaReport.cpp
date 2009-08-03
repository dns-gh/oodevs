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
    temp.truncate(100);
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
void XmliaReport::show()
{
  QDialog* view = new QDialog( );
  view->setMinimumSize(600, 600);
  view->setMaximumSize(600, 600);

  QTextEdit* text = new QTextEdit( view);
  text->setReadOnly( true );
  text->setText( report_.toString() );
  text->setMinimumSize( 600, 600);
  text->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
  text->setColor( QColor( 253, 245, 217 ) );
  view->setBackgroundColor( QColor( 253, 245, 217 ) );
  view->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
  view->setCaption( "XMLIA message details" );
  view->show();
}
