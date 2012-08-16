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
#include "excel/ExcelFormat.h"

using namespace ExcelFormat;

//-----------------------------------------------------------------------------
// Name: ADN_Table constructor
// Created: JDY 03-07-07
//-----------------------------------------------------------------------------
ADN_Table::ADN_Table( QWidget* pParent, const char* szName )
    : Q3Table            ( pParent, szName )
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
void ADN_Table::AdjustColumns( int nMinWidth /*= -1*/ )
{
    for( int n = 0; n < numCols(); ++n )
    {
        adjustColumn( n );
        if( nMinWidth != -1 )
            setColumnWidth( n, std::max< int >( columnWidth( n ) + 5, nMinWidth ) );
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

// -----------------------------------------------------------------------------
// Name: ADN_Table::SaveToXls
// Created: ABR 2012-02-06
// -----------------------------------------------------------------------------
void ADN_Table::SaveToXls( const QString& path, const QString& sheetName ) const
{
    BasicExcel xls;
    xls.New(1);
    xls.RenameWorksheet( 0, sheetName.toAscii().constData() );
    BasicExcelWorksheet* sheet = xls.GetWorksheet(0);
    XLSFormatManager fmt_mgr(xls);
    Workbook::Palette& palette = xls.workbook_.palette_;
    std::vector< std::vector < bool > > mergedCells( numRows(), std::vector< bool >( numCols(), false ) );
    std::vector< int > columnMaxContentSize( numCols(), 0 );

    // Colors
    palette.SetColor( 0, 200, 200, 200 ); // Header colors
    std::pair< double, double > minMax( 0.f, 0.f );
    bool useColor = false;
    for( int row = 0; row < numRows() && !useColor; ++row )
        for( int col = 0; col < numCols() && !useColor; ++col )
        {
            Q3TableItem* qItem = item( row, col );
            if( dynamic_cast< ADN_TableItem_ABC* >( qItem ) != 0 )
            {
                ADN_TableItem_ABC* tableItem = static_cast< ADN_TableItem_ABC* >( qItem );
                if( tableItem->UseColor() )
                {
                    minMax = tableItem->GetRangeForColor();
                    //if( minMax.first != minMax.second )
                    useColor = true;
                }
            }
        }
    double gap = ( useColor ) ? ( minMax.second - minMax.first ) / static_cast< double >( PALETTE_SIZE - 1 ) : 0.f;
    if( useColor )   // Color found, overload palette color with the same algorithm used in ADN_TableItem_Edit
    {
        double value = minMax.first;
        // 55 colors available, from 1 to 55 (0 is taken by header background)
        for( USHORT i = 1; i < PALETTE_SIZE; ++i, value += gap )
        {
            double rRatio = ( value - minMax.first ) / ( minMax.second - minMax.first );
            rRatio = std::max< double >( 0.0, rRatio );
            rRatio = std::min< double >( 1.0, rRatio );

            QColor color;
            color.setHsv( int( 120 * rRatio ), 30, 255 );
            palette.SetColor( i, static_cast< USHORT >( color.blue() ), static_cast< USHORT >( color.green() ), static_cast< USHORT >( color.red() ) );
        }
    }

    // Fill table
    for( int row = 0; row < numRows(); ++row )
    {
        for( int col = 0; col < numCols(); ++col )
        {
            // Border
            CellFormat format( fmt_mgr );
            ExcelFormat::EXCEL_LS left   = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS right  = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS top    = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS bottom = EXCEL_LS_THIN;
            if( col == numCols() - 1 )
                right = EXCEL_LS_MEDIUM;
            if( row == numRows() - 1 )
                bottom = EXCEL_LS_MEDIUM;
            if( vBoldGridRowIndexes_.find( row ) != vBoldGridRowIndexes_.end() || row == 0 )
                top = EXCEL_LS_MEDIUM;
            if( vBoldGridColIndexes_.find( col ) != vBoldGridColIndexes_.end() || col == 0 )
                left = EXCEL_LS_MEDIUM;
            format.set_borderlines( left, right, top, bottom, EGA_BLACK, EGA_BLACK );

            // If Merged, apply border and continue
            Q3TableItem* qItem = item( row, col );
            if( mergedCells[ row ][ col ] || !qItem )
            {
                BasicExcelCell* cell = sheet->Cell(row, col);
                cell->SetFormat( format );
                continue;
            }

            ADN_TableItem_ABC* tableItem = 0;
            if( dynamic_cast< ADN_TableItem_ABC* >( qItem ) != 0 )
                tableItem = static_cast< ADN_TableItem_ABC* >( qItem );
            BasicExcelCell* cell = sheet->Cell(row, col);
            // Content
            QString content = qItem->text();
            bool ok = false;
            content.toInt( &ok );
            if( ok )
                cell->Set( content.toInt() );
            else
            {
                content.toDouble( &ok );
                if( ok )
                    cell->Set( content.toDouble() );
                else
                    cell->Set( content.toAscii().constData() );
            }

            // Column size
            if( columnMaxContentSize[ col ] < content.size() )
                columnMaxContentSize[ col ] = content.size();

            // Merge
            if( qItem->rowSpan() != 1  || qItem->colSpan() != 1 )
            {
                sheet->MergeCells( row, col, static_cast< USHORT >( qItem->rowSpan() ), static_cast< USHORT >( qItem->colSpan() ) );
                format.set_alignment( EXCEL_VALIGN_CENTRED );
                for( int i = 0; i < qItem->rowSpan(); ++i )
                    for( int j = 0; j < qItem->colSpan(); ++j )
                        mergedCells[ row + i ][ col + j ] = true;
            }

            // Header
            if( col == 0 && verticalHeader()->isVisible() || row == 0 && horizontalHeader()->isVisible() )
            {
                format.set_font(ExcelFont().set_weight(FW_BOLD));
                format.set_alignment( EXCEL_HALIGN_CENTRED | EXCEL_VALIGN_CENTRED );
                format.set_background( palette.GetColor( 0 ) );
            }

            // Color
            if( useColor && tableItem && tableItem->UseColor() )
            {
                bool ok = false;
                USHORT index = static_cast< USHORT >( content.toDouble( &ok ) / gap ) + 1;
                //assert( ok && index > 0 && index < PALETTE_SIZE );
                assert( ok );
                if( index == 0 )
                    index = 1;
                else if( index >= PALETTE_SIZE )
                    index = PALETTE_SIZE - 1;
                format.set_background( palette.GetColor( index ) );
            }

            cell->SetFormat( format );
        }
    }

    // Columns width
    static int charactereSize = 300;
    static int minimumSize = 1000;
    for( int col = 0; col < numCols(); ++col )
        sheet->SetColWidth( col, static_cast< USHORT >( std::max( columnMaxContentSize[ col ] * charactereSize, minimumSize ) ) );

    xls.SaveAs( path.toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetToolTips
// Created: ABR 2012-02-16
// -----------------------------------------------------------------------------
QString ADN_Table::GetToolTips( int nRow, int nCol ) const
{
    assert( nRow >= 0 && nRow < numRows() && nCol >= 0 && nCol < numCols() );

    Q3TableItem* it = item( nRow, nCol );
    if( it )
        return it->text();
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::event
// Created: ABR 2012-02-16
// -----------------------------------------------------------------------------
bool ADN_Table::event(QEvent *event)
{
    if( event->type() == QEvent::ToolTip )
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

        const int yPos = helpEvent->pos().y();
        const int xPos = helpEvent->pos().x();
        bool mouseOnHHeader = false;
        bool mouseOnVHeader = false;
        int row = 0;
        int col = 0;

        if( horizontalHeader()->isVisible() )
        {
            if( yPos <= horizontalHeader()->height() )
                mouseOnHHeader = true;
            else
                row = rowAt( yPos - horizontalHeader()->height() + verticalScrollBar()->sliderPosition() );
        }
        else
            row = rowAt( yPos + verticalScrollBar()->sliderPosition() );

        if( verticalHeader()->isVisible() )
        {
            if( xPos <= verticalHeader()->width() )
                mouseOnVHeader = true;
            else
                col = columnAt( xPos - verticalHeader()->width() + horizontalScrollBar()->sliderPosition() );
        }
        else
            col = columnAt( xPos + horizontalScrollBar()->sliderPosition() );

        // Outside, or on both header.
        if( row == -1 || col == -1 || ( mouseOnHHeader && mouseOnVHeader ) )
        {
            QToolTip::hideText();
            event->ignore();
        }
        else
        {
            QString toolTips;
            if( mouseOnHHeader )
                toolTips = horizontalHeader()->label( col );
            else if( mouseOnVHeader )
                toolTips = verticalHeader()->label( row );
            else
                toolTips = GetToolTips( row, col );

            if( !toolTips.isEmpty() )
                QToolTip::showText( helpEvent->globalPos(), toolTips );
        }
        return true;
    }
    return Q3Table::event(event);
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::eventFilter
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
bool ADN_Table::eventFilter( QObject * watched, QEvent * event )
{
    if( QMouseEvent* mouseEvent = dynamic_cast< QMouseEvent* >( event ) )
        if( mouseEvent && ( mouseEvent->button() == Qt::XButton1 || mouseEvent->button() == Qt::XButton2 ) )
            return false;
    return Q3Table::eventFilter( watched, event );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::SetGoToOnDoubleClick
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_Table::SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab /*= -1*/, int col /*= 0*/ )
{
    goToInfo_.targetTab_ = targetTab;
    goToInfo_.subTargetTab_ = subTargetTab;
    goToInfo_.sourceColumn_ = col;
    connect( this, SIGNAL( doubleClicked( int, int, int, const QPoint& ) ), SLOT( GoToOnDoubleClicked( int, int, int, const QPoint& ) ) );
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GoToOnDoubleClicked
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_Table::GoToOnDoubleClicked( int row, int col, int button, const QPoint& )
{
    if( button != Qt::LeftButton || !( row >= 0 && row < numRows() && col >= 0 && col < numCols() ) || col != goToInfo_.sourceColumn_ )
        return;
    Q3TableItem* qItem = item( row, col );
    if( !qItem )
        return;
    goToInfo_.targetName_ = qItem->text();
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}

