//*****************************************************************************
//
// $Created: JDY 03-07-07 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Table.cpp $
// $Author: Nld $
// $Modtime: 10/05/05 10:58 $
// $Revision: 11 $
// $Workfile: ADN_Table.cpp $
//
//*****************************************************************************

#include  "adaptation_app_pch.h"
#include "ADN_Table.h"
#include "moc_ADN_Table.cpp"
#include "ADN_TableItem_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Connector_Table_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_Table constructor
// Created: JDY 03-07-07
//-----------------------------------------------------------------------------
ADN_Table::ADN_Table( QWidget* pParent, const char* szName )
    : Q3Table             ( pParent, szName )
    , ADN_Gfx_ABC        ()
    , bRefreshingEnabled_( true )
    , bPrinting_         ( false )
{
    connect( this, SIGNAL( valueChanged( int, int) ), this, SLOT( doValueChanged( int, int ) ) );

    connect( this, SIGNAL( contextMenuRequested ( int, int, const QPoint &) ), this, SLOT( OnContextMenu( int, int, const QPoint & ) ) );

    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Table destructor
// Created: JDY 03-07-07
//-----------------------------------------------------------------------------
ADN_Table::~ADN_Table()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::paintCell
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
void ADN_Table::paintCell( QPainter* p, int nRow, int nCol, const QRect& cr, bool bSelected, const QColorGroup& cg )
{
    if( bRefreshingEnabled_ )
        Q3Table::paintCell( p, nRow, nCol, cr, bSelected, cg );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::paintCell
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
void ADN_Table::paintCell( QPainter* p, int nRow, int nCol, const QRect& cr, bool bSelected )
{
    if( bRefreshingEnabled_ )
        Q3Table::paintCell( p, nRow, nCol, cr, bSelected );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::doValueChanged
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Table::doValueChanged ( int row, int col )
{
    static_cast<ADN_TableItem_ABC*>( item(row,col) )->DoValueChanged();
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::OnContextMenu
// Created: JDY 03-07-29
//-----------------------------------------------------------------------------
void ADN_Table::OnContextMenu( int /*row*/, int /*col*/, const QPoint& /*pt*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::paintEmptyArea
// Created: APE 2005-04-01
// -----------------------------------------------------------------------------
void ADN_Table::paintEmptyArea( QPainter* pPainter, int nX, int nY, int nWidth, int nHeight )
{
    // Prevent the function from being called when printing.
    if( ! bPrinting_ )
        Q3Table::paintEmptyArea( pPainter, nX, nY, nWidth, nHeight );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AdjustColumns
// Created: APE 2005-04-01
// -----------------------------------------------------------------------------
void ADN_Table::AdjustColumns( int nMinWidth )
{
    for( int n = 0; n < this->numCols(); ++n )
    {
        this->adjustColumn( n );
        this->setColumnWidth( n, std::max< int >( this->columnWidth( n ) + 5, nMinWidth ) );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::StopEditing
// Created: JDY 03-07-10
//-----------------------------------------------------------------------------
void ADN_Table::StopEditing()
{
    clearSelection( false );
    if( isEditing() )
        Q3Table::endEdit( currEditRow(), currEditCol(), false, false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::sortColumn
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
void ADN_Table::sortColumn( int nCol, bool bAscending, bool /*wholerows*/ )
{
    Q3Table::sortColumn( nCol, bAscending, true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table::GetCurrentData
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
void* ADN_Table::GetCurrentData()
{
    int nRow = currentRow();
    int nCol = currentColumn();

    if( nRow == -1 || nCol == -1 )
        return 0;

    ADN_TableItem_ABC* pItem = static_cast< ADN_TableItem_ABC* >( item( nRow, nCol ) );
    if( pItem != 0 )
        return pItem->GetData();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_Table::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        Q3Table::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        Q3Table::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_Table::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_Table_ABC* >( pConnector_ )->IsConnected() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::drawContents
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
void ADN_Table::drawContents( QPainter * p, int cx, int cy, int cw, int ch )
{
    Q3Table::drawContents( p, cx, cy, cw, ch );

    QPen pen( p->pen() );
    p->setPen( QPen( Qt::black, 2 ) );
    for( IT_IndexSet it = vBoldGridRowIndexes_.begin(); it != vBoldGridRowIndexes_.end(); ++it )
    {
        int nRowPos = rowPos( *it ) - 1;
        p->drawLine( 0, nRowPos, this->contentsWidth() - 1, nRowPos );
    }
    for( IT_IndexSet it = vBoldGridColIndexes_.begin(); it != vBoldGridColIndexes_.end(); ++it )
    {
        int nColPos = columnPos( *it ) - 1;
        p->drawLine( nColPos, 0, nColPos, this->contentsHeight() - 1 );
    }
    p->setPen( pen );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::ComputeNbrPrintPages
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
int ADN_Table::ComputeNbrPrintPages( const QSize& painterSize ) const
{
    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsWidth() / painterSize.width() ) );
    int nHeightInPages = static_cast< int >( ceil( ( float ) this->contentsHeight() / painterSize.height() ) );
    return nWidthInPages * nHeightInPages;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Print
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_Table::Print( int nPage, QPainter& painter, const QSize& painterSize )
{
    bPrinting_ = true;
    // Ready the table.
    this->clearSelection( true );
    this->setCurrentCell( -1, -1 );

    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsWidth() / painterSize.width() ) );

    int nY = static_cast< int >( floor( ( float ) nPage / nWidthInPages ) );
    int nX = nPage % nWidthInPages;

    painter.save();
    painter.translate( -nX * painterSize.width(), -nY * painterSize.height() );
    this->drawContents( &painter, nX * painterSize.width(), nY * painterSize.height(), painterSize.width(), painterSize.height() );
    painter.restore();
    bPrinting_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Print
// $$$$ APE 2005-05-10: Ugly, clean it up sometime.
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
QWidget *ADN_Table::createEditor( int nRow, int nCol, bool bInitFromCell ) const
{
    if( isReadOnly() || isRowReadOnly( nRow ) || isColumnReadOnly( nCol ) )
        return 0;

    QWidget *e = 0;

    // the current item in the cell should be edited if possible
    Q3TableItem *i = item( nRow, nCol );
    if( bInitFromCell || ( i && !i->isReplaceable() ) ) {
        if( i ) {
            if( i->editType() == Q3TableItem::Never )
                return 0;

            e = i->createEditor();
            if( !e )
                return 0;
        }
    }
    // no contents in the cell yet, so open the default editor
    if( !e ) {
        if( i ) {
            if( i->editType() == Q3TableItem::Never )
                return 0;

            e = i->createEditor();
            if( !e )
                return 0;
        }
        else
        {
            e = new QLineEdit( viewport(), "qt_lineeditor" );
            ( (QLineEdit*)e )->setFrame( FALSE );
        }
    }
    return e;
}