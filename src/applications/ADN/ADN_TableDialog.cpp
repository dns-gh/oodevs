// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableDialog.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 15:53 $
// $Revision: 3 $
// $Workfile: ADN_TableDialog.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_TableDialog.h"
#include "moc_ADN_TableDialog.cpp"

#include <qpopupmenu.h>
#include <qcursor.h>
#include <qlayout.h>
#include <qrect.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qpaintdevicemetrics.h>
#include <qhbox.h>
#include <qpushbutton.h>

#include "ADN_Table.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"


// -----------------------------------------------------------------------------
// Name: ADN_TableDialog constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_TableDialog::ADN_TableDialog( QWidget* pParent, const QString& strCaption, ADN_Table* pTable )
: QDialog( pParent, strCaption )
, pTable_( pTable )
{
    assert( pTable != 0 );
    this->setCaption( strCaption );

    pTable->reparent( this, QPoint(0,0) );
    connect( pTable, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), this, SLOT( OnContextMenu() ) );
    
    QHBox* pHBox = new QHBox( this );
    QPushButton* pPrintButton = new QPushButton( tr( "Print" ), pHBox );
    QPushButton* pCloseButton = new QPushButton( tr( "Close" ), pHBox );
    connect( pPrintButton, SIGNAL( clicked() ), this, SLOT( PrintTable() ) );
    connect( pCloseButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
        
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->addWidget( pTable );
    pLayout->addWidget( pHBox );

    QMainWindow* pMainWindow = ADN_App::pApplication_->GetMainWindow();
    this->resize( pMainWindow->width() * 0.8, pMainWindow->height() * 0.8 );
    this->move( pMainWindow->x() + pMainWindow->width() * 0.1, pMainWindow->y() + pMainWindow->height() * 0.1 );
}


// -----------------------------------------------------------------------------
// Name: ADN_TableDialog destructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_TableDialog::~ADN_TableDialog()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_TableDialog::OnContextMenu
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_TableDialog::OnContextMenu()
{
    QPopupMenu popup( this );
    popup.insertItem( tr( "Print" ), 1 );

    int nResult = popup.exec( QCursor::pos() );

    if( nResult == 1 )
        this->PrintTable();
}


// -----------------------------------------------------------------------------
// Name: ADN_TableDialog::PrintTable
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_TableDialog::PrintTable()
{
    // Setup the printer.
    QPrinter printer;
    if( ! printer.setup() )
        return;
    QPainter painter( &printer );

    // Check that there is a valid device to print to.
    if ( !painter.device() )
        return;

    // Hide headers, show first line (headers as "table cells")
    bool bHeadersShown = !pTable_->horizontalHeader()->isHidden();
    if( bHeadersShown )
    {
        pTable_->horizontalHeader()->hide();
        pTable_->showRow( 0 );
    }

    // Compute the printing rectangle.
    QPaintDeviceMetrics metrics( painter.device() );

    const float rMarginInInches = 0.5f; 
    int nXMargin = rMarginInInches * (float)metrics.logicalDpiX();
    int nYMargin = rMarginInInches * (float)metrics.logicalDpiY();

    QRect pageRect( nXMargin, nYMargin, metrics.width() - nXMargin * 2, metrics.height() - nYMargin * 2 );
    painter.setClipRect( pageRect, QPainter::CoordDevice );
    painter.translate( nXMargin, nYMargin );

    const double rScale = 1;
    painter.scale( rScale, rScale );
    QSize painterSize( (int)(pageRect.width() / rScale) + 1, (int)(pageRect.height() / rScale) + 1 );

    int nNbrPages = pTable_->ComputeNbrPrintPages( painterSize );

    for( int n = 0; n < nNbrPages; ++n )
    {
        if( n != 0 )
            printer.newPage();
        pTable_->Print( n, painter, painterSize );
    }

    // Show headers if needed, hide first line (headers as "table cells")
    if( bHeadersShown )
    {
        pTable_->horizontalHeader()->show();
        pTable_->hideRow( 0 );
    }
}
