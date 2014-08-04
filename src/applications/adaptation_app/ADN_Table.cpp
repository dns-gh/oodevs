// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include  "adaptation_app_pch.h"
#include "ADN_Table.h"
#include "moc_ADN_Table.cpp"
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_Connector_StandardItem.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Languages_GUI.h"
#include "ADN_WorkspaceElement.h"
#include "excel/ExcelFormat.h"
#include "clients_kernel/LanguageController.h"
#include "clients_kernel/VariantPointer.h"

using namespace ExcelFormat;

// -----------------------------------------------------------------------------
// Name: ADN_Table::ADN_Table
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_Table::ADN_Table( const QString& objectName, QWidget* pParent /* = 0 */ )
    : QTableView( pParent )
    , dataModel_( pParent )
    , delegate_ ( this )
{
    Initialize( objectName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::ADN_Table
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
ADN_Table::ADN_Table( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent /* = 0 */ )
    : QTableView( pParent )
    , dataModel_( pParent )
    , delegate_ ( this )
{
    Initialize( objectName );

    pConnector_.reset( new ADN_Connector_Table_ABC( *this ) );
    pConnector_->Connect( &vector );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::ADN_Table
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
ADN_Table::ADN_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : QTableView( pParent )
    , dataModel_( pParent )
    , delegate_ ( this )
{
    Initialize( objectName );

    pConnector_.reset( new ADN_Connector_Table_ABC( *this ) );
    connector = pConnector_.get();
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Initialize
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Table::Initialize( const QString& objectName )
{
    setObjectName( objectName );
    setContextMenuPolicy( Qt::CustomContextMenu );
    proxyModel_.reset( new QSortFilterProxyModel( this ) );
    proxyModel_->setSortLocaleAware( true );
    proxyModel_->setSourceModel( &dataModel_ );
    proxyModel_->setSortRole( gui::Roles::DataRole );
    setModel( proxyModel_.get() );
    setItemDelegate( &delegate_ );
    setEditTriggers( AllEditTriggers );

    setSortingEnabled( true );
    setShowGrid( false );
    setAlternatingRowColors( true );
    verticalHeader()->setDefaultSectionSize( 20 );

    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectItems );
    setEditTriggers( AllEditTriggers );

    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), &delegate_, SLOT( OnItemChanged( QStandardItem* ) ) );
    connect( &delegate_, SIGNAL( CheckedStateChanged( const QStandardItem& ) ), this, SLOT( OnCheckedStateChanged( const QStandardItem& ) ) );
    connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( PrivateOnContextMenu( const QPoint& ) ) );

    ADN_Workspace::GetWorkspace().GetLanguageController().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::~ADN_Table
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
ADN_Table::~ADN_Table()
{
    // NOTHING
    ADN_Workspace::GetWorkspace().GetLanguageController().Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::numRows
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
int ADN_Table::numRows() const
{
    return dataModel_.rowCount();
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::setNumRows
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
void ADN_Table::setNumRows( int numRows )
{
    selectionModel()->clearSelection();
    setCurrentIndex( QModelIndex() );
    dataModel_.setRowCount( numRows );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddRow
// Created: ABR 2012-10-19
// -----------------------------------------------------------------------------
void ADN_Table::AddRow( int /* row */, void* /* data */ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::RemoveItem
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
void ADN_Table::RemoveItem( void* pData )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row, 0 ) );
        if( item->GetData() == pData )
        {
            dataModel_.removeRow( row );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::RemoveCurrentElement
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
void ADN_Table::RemoveCurrentElement()
{
    void* data = GetSelectedData();
    if( data )
        static_cast< ADN_Connector_Vector_ABC& >( GetConnector() ).RemItem( data );
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
    delete static_cast< ADN_Ref_ABC* >( data );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::PrivateOnContextMenu
// Created: ABR 2012-11-08
// -----------------------------------------------------------------------------
void ADN_Table::PrivateOnContextMenu( const QPoint& pt )
{
    OnContextMenu( viewport()->mapToGlobal( pt ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::OnContextMenu
// Created: ABR 2012-10-29
// -----------------------------------------------------------------------------
void ADN_Table::OnContextMenu( const QPoint& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::OnCheckedStateChanged
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
void ADN_Table::OnCheckedStateChanged( const QStandardItem& item )
{
    const ADN_StandardItem& adnItem = static_cast< const ADN_StandardItem& >( item );
    assert( adnItem.GetType() == ADN_StandardItem::eBool );
    ADN_Connector_StandardItem* connector = static_cast< ADN_Connector_StandardItem* >( adnItem.GetConnector() );
    if( connector )
        connector->SetDataChanged( "" );
}

namespace
{
    template< typename T >
    QColor GetColorFromValue( const std::pair< double, double >& minMax, T value )
    {
        double rRatio = ( value - minMax.first ) / ( minMax.second - minMax.first );
        rRatio = std::min< double >( 1.0, std::max< double >( 0.0, rRatio ) );
        QColor backgroundColor;
        backgroundColor.setHsv( int( 120 * rRatio ), 30, 255 );
        return backgroundColor;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::dataChanged
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
void ADN_Table::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    if( topLeft == bottomRight )
    {
        bool ok = false;
        QStandardItem* item = GetItemFromIndex( topLeft );
        if( !item )
            return;
        double value = item->data( gui::Roles::DataRole ).toDouble( &ok );
        if( !ok )
            return;
        if( const std::pair< double, double >* colorGap = delegate_.GetColorType( item->row(), item->column() ) )
            item->setData( GetColorFromValue( *colorGap, value ), Qt::BackgroundColorRole );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::SetGoToOnDoubleClick
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void ADN_Table::SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab /*= -1*/, int col /* = 0 */ )
{
    goToInfo_.targetTab_ = targetTab;
    goToInfo_.subTargetTab_ = subTargetTab;
    goToInfo_.sourceColumn_ = col;
    connect( this, SIGNAL( activated( const QModelIndex& ) ), this, SLOT( OnGotoRequested( const QModelIndex& ) ) );
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetItemFromIndex
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
QStandardItem* ADN_Table::GetItemFromIndex( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return 0;
    return dataModel_.itemFromIndex( index.model() == &dataModel_ ? index : proxyModel_->mapToSource( index ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetItem
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
QStandardItem* ADN_Table::GetItem( int row, int col ) const
{
    return GetItemFromIndex( proxyModel_->index( row, col ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::OnGotoRequested
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void ADN_Table::OnGotoRequested( const QModelIndex& index )
{
    QStandardItem* item = GetItemFromIndex( index );
    if( !item || item->column() != goToInfo_.sourceColumn_ )
        return;

    goToInfo_.targetName_ = item->data( Qt::EditRole ).toString();
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetDataFromIndex
// Created: ABR 2012-10-29
// -----------------------------------------------------------------------------
void* ADN_Table::GetDataFromIndex( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return 0;
    ADN_StandardItem* oldItem = static_cast< ADN_StandardItem* >( GetItemFromIndex( index ) );
    if( !oldItem )
        return 0;
    return oldItem->GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetData
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void* ADN_Table::GetData( int row, int col ) const
{
    if( row < 0 || row >= dataModel_.rowCount() || col < 0 || col >= dataModel_.columnCount() )
        return 0;
    ADN_StandardItem* item = static_cast< ADN_StandardItem* >( GetItem( row, col ) );
    if( !item )
        return 0;
    return item->GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetCurrentData
// Created: ABR 2012-10-29
// -----------------------------------------------------------------------------
void* ADN_Table::GetSelectedData() const
{
    QModelIndexList indexes = selectedIndexes();
    if( indexes.size() > 0 )
        return GetDataFromIndex( indexes[ 0 ] );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetData
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
void* ADN_Table::GetData( const QPoint& pt )
{
    if( !viewport() )
        return 0;
    QModelIndex index = indexAt( viewport()->mapFromGlobal( pt ) );
    if( index.isValid() )
       return GetDataFromIndex( index );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddBoldGridRow
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_Table::AddBoldGridRow( int nIndex )
{
    delegate_.AddBoldRowIndex( nIndex );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddBoldGridCol
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_Table::AddBoldGridCol( int nIndex )
{
    delegate_.AddBoldColumnIndex( nIndex );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::ComputeNbrPrintPages
// Created: JSR 2012-11-07
// -----------------------------------------------------------------------------
int ADN_Table::ComputeNbrPrintPages( const QSize& /*painterSize*/ ) const
{
    // TODO
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Print
// Created: JSR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_Table::Print( int /*nPage*/, QPainter& /*painter*/, const QSize& /*painterSize*/ )
{
    // TODO
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::SaveToXls
// Created: JSR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_Table::SaveToXls( const tools::Path& path, const QString& sheetName ) const
{
    BasicExcel xls;
    xls.New(1);
    xls.RenameWorksheet( 0, sheetName.toUtf8().constData() );
    BasicExcelWorksheet* sheet = xls.GetWorksheet(0);
    XLSFormatManager fmt_mgr(xls);
    Workbook::Palette& palette = xls.workbook_.palette_;
    std::vector< std::vector < bool > > mergedCells( dataModel_.rowCount(), std::vector< bool >( dataModel_.columnCount(), false ) );
    std::vector< int > columnMaxContentSize( dataModel_.columnCount(), 0 );

    // Colors
    palette.SetColor( 0, 200, 200, 200 ); // Header colors
    std::pair< double, double > minMax( 0.f, 0.f );
    bool useColor = false;
    for( int row = 0; row < dataModel_.rowCount() && !useColor; ++row )
    {
        for( int col = 0; col < dataModel_.columnCount() && !useColor; ++col )
        {
            if( const std::pair< double, double >* color = delegate_.GetColorType( row, col ) )
            {
                minMax = *color;
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

    // Header
    int baseCellRow = 0;
    if( horizontalHeader()->isVisible() )
    {
        baseCellRow = 1;
        for( int col = 0; col < dataModel_.columnCount(); ++col )
        {
            QStandardItem* item = dataModel_.horizontalHeaderItem( col );
            if( !item )
                continue;

            QString content = item->text();
            CellFormat format( fmt_mgr );

            format.set_borderlines( EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EGA_BLACK, EGA_BLACK );
            format.set_font(ExcelFont().set_weight(FW_BOLD));
            format.set_alignment( EXCEL_HALIGN_CENTRED | EXCEL_VALIGN_CENTRED );
            format.set_background( palette.GetColor( 0 ) );

            BasicExcelCell* cell = sheet->Cell( 0, col );
            cell->Set( content.toStdWString().c_str() );

            // Column size
            if( columnMaxContentSize[ col ] < content.size() )
                columnMaxContentSize[ col ] = content.size();

            cell->SetFormat( format );
        }
    }

    // Fill table
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        // Handle vertical header here, when that will be needed.
        for( int col = 0; col < dataModel_.columnCount(); ++col )
        {
            // Border
            CellFormat format( fmt_mgr );
            ExcelFormat::EXCEL_LS left   = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS right  = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS top    = EXCEL_LS_THIN;
            ExcelFormat::EXCEL_LS bottom = EXCEL_LS_THIN;
            if( col == dataModel_.columnCount() - 1 )
                right = EXCEL_LS_MEDIUM;
            if( row == dataModel_.rowCount() - 1 )
                bottom = EXCEL_LS_MEDIUM;
            if( delegate_.GetBoldRowIndexes().find( row ) != delegate_.GetBoldRowIndexes().end() )
                top = EXCEL_LS_MEDIUM;
            if( delegate_.GetBoldColumnIndexes().find( col ) != delegate_.GetBoldColumnIndexes().end() || col == 0 )
                left = EXCEL_LS_MEDIUM;
            format.set_borderlines( left, right, top, bottom, EGA_BLACK, EGA_BLACK );

            // If Merged, apply border and continue
            QStandardItem* qItem = GetItemFromIndex( proxyModel_->index( row, col ) );
            if( mergedCells[ row ][ col ] || !qItem )
            {
                BasicExcelCell* cell = sheet->Cell( row + baseCellRow, col );
                cell->SetFormat( format );
                continue;
            }

            BasicExcelCell* cell = sheet->Cell( row + baseCellRow, col );
            // Content
            QString content = qItem->text();
            bool ok = false;
            cell->Set( locale().toInt( content, &ok ) );
            if( !ok )
            {
                cell->Set( locale().toDouble( content, &ok ) );
                if( !ok )
                    cell->Set( content.toStdWString().c_str() );
            }
            // Column size
            if( columnMaxContentSize[ col ] < content.size() )
                columnMaxContentSize[ col ] = content.size();

            // Merge
            int currentRowSpan = rowSpan( row, col );
            int currentColSpan = columnSpan( row, col );
            if( currentRowSpan != 1 || currentColSpan != 1 )
            {
                sheet->MergeCells( row + baseCellRow, col, static_cast< USHORT >( currentRowSpan ), static_cast< USHORT >( currentColSpan ) );
                format.set_alignment( EXCEL_VALIGN_CENTRED );
                for( int i = 0; i < currentRowSpan; ++i )
                    for( int j = 0; j <currentColSpan; ++j )
                        mergedCells[ row + i ][ col + j ] = true;
            }

            // Color
            if( useColor && delegate_.GetColorType( row, col ) != 0 )
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
    for( int col = 0; col < dataModel_.columnCount(); ++col )
        sheet->SetColWidth( col, static_cast< USHORT >( std::max( columnMaxContentSize[ col ] * charactereSize, minimumSize ) ) );

    xls.SaveAs( path.ToUnicode().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::event
// Created: ABR 2012-02-16
// -----------------------------------------------------------------------------
bool ADN_Table::event( QEvent *event )
{
    if( event->type() == QEvent::ToolTip )
    {
        QHelpEvent *helpEvent = static_cast< QHelpEvent* >( event );
        if( !helpEvent )
            return QTableView::event( event );

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
            {
                QStandardItem* item = dataModel_.horizontalHeaderItem( col );
                if( item )
                    toolTips = item->text();
            }
            else if( mouseOnVHeader )
            {
                QStandardItem* item = dataModel_.verticalHeaderItem( row );
                if( item )
                    toolTips = item->text();
            }
            else
                toolTips = GetToolTips( row, col );

            if( !toolTips.isEmpty() )
                QToolTip::showText( helpEvent->globalPos(), toolTips );
        }
        return true;
    }
    return QTableView::event( event );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::mousePressEvent
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_Table::mousePressEvent( QMouseEvent* event )
{
    if( event && ( event->button() == Qt::XButton1 || event->button() == Qt::XButton2 ) )
        event->ignore();
    else
        QTableView::mousePressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::GetToolTips
// Created: ABR 2012-02-16
// -----------------------------------------------------------------------------
QString ADN_Table::GetToolTips( int nRow, int nCol ) const
{
    assert( nRow >= 0 && nRow < dataModel_.rowCount() && nCol >= 0 && nCol < dataModel_.columnCount() );
    QStandardItem* item = GetItem( nRow, nCol );
    if( item )
        return item->text();
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Sort
// Created: ABR 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Table::Sort( int column /* = 0 */, Qt::SortOrder order /* = Qt::AscendingOrder */ )
{
    proxyModel_->sort( column, order );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::CheckValidity
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void ADN_Table::CheckValidity( int row, int col /* = -1 */ )
{
    if( ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row, 0 ) ) ) // each column have the same parent data
    {
        QObject* parentData = reinterpret_cast< QObject* >( item->GetData() );
        if( ADN_Ref_ABC* parentRef = dynamic_cast< ADN_Ref_ABC* >( parentData ) )
            parentRef->CheckValidity();
    }
    if( col == -1 )
        for( col = 0; col < dataModel_.columnCount(); ++col )
            Warn( row, col );
    else
        Warn( row, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::Warn
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void ADN_Table::Warn( int row, int col )
{
    if( ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row, col ) ) )
        if( const ADN_Ref_ABC* itemData = reinterpret_cast< const ADN_Ref_ABC* >( item->data( gui::Roles::SafeRole ).value< kernel::VariantPointer >().ptr_ ) )
            item->Warn( itemData->GetErrorStatus() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::OnLanguageChanged
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void ADN_Table::OnLanguageChanged()
{
    setFocus();
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        for( int col = 0; col < dataModel_.columnCount(); ++col )
            Warn( row, col );
}
