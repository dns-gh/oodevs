// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListViewDialog.h"
#include "moc_ADN_ListViewDialog.cpp"
#include "ADN_ListView.h"
#include "ADN_Workspace.h"
#include "ADN_MainWindow.h"
#include "clients_gui/FileDialog.h"
#include <shellapi.h>

// -----------------------------------------------------------------------------
// Name: ADN_ListViewDialog constructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_ListViewDialog::ADN_ListViewDialog( QWidget* pParent, const QString& strCaption, ADN_ListView& pListView )
    : QDialog( pParent, strCaption )
    , pListView_ ( pListView )
    , strCaption_( strCaption )
{
    this->setCaption( strCaption );
    pListView.reparent( this, QPoint(0,0) );
    Q3HBox* pHBox = new Q3HBox( this );
    QPushButton* pSaveButton = new QPushButton( tr( "Save" ), pHBox );
    //QPushButton* pPrintButton = new QPushButton( tr( "Print" ), pHBox );
    QPushButton* pCloseButton = new QPushButton( tr( "Close" ), pHBox );
    connect( pSaveButton, SIGNAL( clicked() ), this, SLOT( SaveListView() ) );
    //connect( pPrintButton, SIGNAL( clicked() ), this, SLOT( PrintListView() ) );
    connect( pCloseButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( this );
    pLayout->addWidget( &pListView );
    pLayout->addWidget( pHBox );
    QMainWindow& mainWindow = ADN_Workspace::GetWorkspace().GetMainWindow();
    this->resize( static_cast< int >( mainWindow.width() * 0.8 ), static_cast< int >( mainWindow.height() * 0.8 ) );
    this->move( mainWindow.x() + static_cast< int >( mainWindow.width() * 0.1 ), mainWindow.y() + static_cast< int >( mainWindow.height() * 0.1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListViewDialog destructor
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
ADN_ListViewDialog::~ADN_ListViewDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListViewDialog::PrintListView
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_ListViewDialog::PrintListView()
{
    // Setup the printer.
    QPrinter printer;
    if( ! printer.setup() )
        return;
    QPainter painter( &printer );
    // Check that there is a valid device to print to.
    if( !painter.device() )
        return;
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
    QSize painterSize( (int)(pageRect.width() / rScale) + 1, (int)(pageRect.height() / rScale) + 1 );
    const int nNbrPages = pListView_.ComputeNbrPrintPages( painterSize );
    for( int n = 0; n < nNbrPages; ++n )
    {
        if( n != 0 )
            printer.newPage();
        pListView_.Print( n, painter, painterSize );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListViewDialog::SaveListView
// Created: ABR 2012-02-09
// -----------------------------------------------------------------------------
void ADN_ListViewDialog::SaveListView()
{
    const tools::Path path = gui::FileDialog::getSaveFileName( this, tr( "Save" ), tools::Path(), tr("Excel files (*.xls)") );
    if( path.IsEmpty() )
        return;
    pListView_.SaveToXls( path, strCaption_ );
    ShellExecuteW( 0, NULL, path.ToUnicode().c_str(), NULL, NULL, SW_NORMAL );
}
