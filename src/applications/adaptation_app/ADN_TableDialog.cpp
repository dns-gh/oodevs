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

#include "adaptation_app_pch.h"
#include "ADN_TableDialog.h"
#include "moc_ADN_TableDialog.cpp"
#include "ADN_Table.h"
#include "ADN_MainWindow.h"
#include "clients_gui/FileDialog.h"
#include <shellapi.h>

// -----------------------------------------------------------------------------
// Name: ADN_TableDialog constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_TableDialog::ADN_TableDialog( QWidget* pParent, const QString& strCaption, ADN_Table& table )
    : QDialog( pParent, strCaption )
    , caption_( strCaption )
    , table_( table )
{
    this->setCaption( strCaption );

    table_.reparent( this, QPoint(0,0) );

    Q3HBox* pHBox = new Q3HBox( this );
    QPushButton* pSaveButton = new QPushButton( tr( "Save" ), pHBox );
    //QPushButton* pPrintButton = new QPushButton( tr( "Print" ), pHBox );
    QPushButton* pCloseButton = new QPushButton( tr( "Close" ), pHBox );
    connect( pSaveButton, SIGNAL( clicked() ), this, SLOT( SaveTable() ) );
    //connect( pPrintButton, SIGNAL( clicked() ), this, SLOT( PrintTable() ) );
    connect( pCloseButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    Q3VBoxLayout* pLayout = new Q3VBoxLayout( this );
    pLayout->addWidget( &table_ );
    pLayout->addWidget( pHBox );

    QMainWindow& mainWindow = ADN_Workspace::GetWorkspace().GetMainWindow();
    this->resize( static_cast< int >( mainWindow.width() * 0.8 ), static_cast< int >( mainWindow.height() * 0.8 ) );
    this->move( mainWindow.x() + static_cast< int >( mainWindow.width() * 0.1 ), mainWindow.y() +  static_cast< int >( mainWindow.height() * 0.1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDialog destructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_TableDialog::~ADN_TableDialog()
{
    // NOTHING
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
    if( !painter.device() )
        return;

    // Hide headers, show first line (headers as "table cells")
    bool bHeadersShown = !table_.horizontalHeader()->isHidden();
    if( bHeadersShown )
    {
        table_.horizontalHeader()->hide();
        table_.showRow( 0 );
    }

    // Compute the printing rectangle.
    Q3PaintDeviceMetrics metrics( painter.device() );

    const float rMarginInInches = 0.5f;
    const int nXMargin = static_cast< int >( rMarginInInches * metrics.logicalDpiX() );
    const int nYMargin = static_cast< int >( rMarginInInches * metrics.logicalDpiY() );

    QRect pageRect( nXMargin, nYMargin, metrics.width() - nXMargin * 2, metrics.height() - nYMargin * 2 );
    painter.setClipRect( pageRect, Qt::ReplaceClip );
    painter.translate( nXMargin, nYMargin );

    const double rScale = 1;
    painter.scale( rScale, rScale );
    QSize painterSize( static_cast< int >( pageRect.width() / rScale ) + 1, static_cast< int >( pageRect.height() / rScale ) + 1 );

    const int nNbrPages = table_.ComputeNbrPrintPages( painterSize );

    for( int n = 0; n < nNbrPages; ++n )
    {
        if( n != 0 )
            printer.newPage();
        table_.Print( n, painter, painterSize );
    }

    // Show headers if needed, hide first line (headers as "table cells")
    if( bHeadersShown )
    {
        table_.horizontalHeader()->show();
        table_.hideRow( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDialog::SaveTable
// Created: ABR 2012-01-27
// -----------------------------------------------------------------------------
void ADN_TableDialog::SaveTable()
{
    const tools::Path path = gui::FileDialog::getSaveFileName( this, tr( "Save" ), tools::Path(), tr("Excel files (*.xls)") );
    if( path.IsEmpty() )
        return;
    table_.SaveToXls( path, caption_ );
    ShellExecuteW( 0, NULL, path.ToUnicode().c_str(), NULL, NULL, SW_NORMAL );
}
