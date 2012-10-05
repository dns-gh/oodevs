//*****************************************************************************
//
// $Created: JDY 03-06-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 10:43 $
// $Revision: 10 $
// $Workfile: ADN_ListView.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView.h"
#include "moc_ADN_ListView.cpp"
#include "ADN_ListViewItem.h"
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Tools.h"
#include "ADN_GuiTools.h"
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_MainWindow.h"
#include "ADN_ListViewToolTip.h"
#include <boost/bind.hpp>
#include <excel/ExcelFormat.h>

using namespace ExcelFormat;

//-----------------------------------------------------------------------------
// Name: ADN_ListView constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::ADN_ListView( QWidget* pParent, const char* szName, const QString title /*= ""*/ ) // flags � virer
    : gui::RichTreeView( pParent )
    , ADN_Gfx_ABC       ()
    , pCurData_         ( 0 )
    , title_            ( title )
    , pObjectCreator_   ( 0 )
    , bDeletionEnabled_ ( false )
    , bDeletionWarning_ ( true )
    , bPrinting_        ( false )
    , usedByMapper_     ( this )
{
    setObjectName( szName );
    setRootIsDecorated( false );
    if( !title.isEmpty() )
        dataModel_.setHorizontalHeaderLabels( QStringList( title ) );
    viewport()->installEventFilter( new ADN_ListViewToolTip( viewport(), *this ) );

    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( SetCurrentItem() ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
    connect( &usedByMapper_, SIGNAL( mapped( int ) ), this, SLOT( ContextMenuSearchElements( int ) ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::~ADN_ListView()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::ItemAt
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::ItemAt( int i )
{
    assert( i < dataModel_.rowCount() );
    return static_cast< ADN_ListViewItem* >( dataModel_.item( i ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem( void* data )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( i ) );
        if( pCurr->GetData() == data )
            return pCurr;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem( const QString& itemName  )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( i ) );
        if( pCurr->text() == itemName )
            return pCurr;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::InsertItem
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::InsertItem( ADN_ListViewItem* item )
{
    dataModel_.appendRow( item );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::InsertItems
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::InsertItems( const QList< QStandardItem* >& items )
{
    dataModel_.appendRow( items );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::TakeItem
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::TakeItem( ADN_ListViewItem* item )
{
    const QModelIndex index = dataModel_.indexFromItem( item );
    dataModel_.removeRow( index.row(), index.parent() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::MoveItem
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::MoveItem( ADN_ListViewItem* src, ADN_ListViewItem* dest )
{
    dataModel_.insertRow( dest->row() + 1, dataModel_.takeRow( src->row() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::MoveItemAbove
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::MoveItemAbove( ADN_ListViewItem* src, ADN_ListViewItem* dest )
{
    dataModel_.insertRow( dest->row(), dataModel_.takeRow( src->row() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Swap
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::Swap( ADN_ListViewItem* src, ADN_ListViewItem* dest )
{
    int destSrc = dest->row();
    MoveItem( dest, src );
    dataModel_.insertRow( destSrc, dataModel_.takeRow( src->row() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ChildCount
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
int ADN_ListView::ChildCount() const
{
    return dataModel_.rowCount();
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Clear
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::Clear()
{
    dataModel_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::CreateTableFrom
// Created: JSR 2012-10-03
// -----------------------------------------------------------------------------
void ADN_ListView::CreateTableFrom( std::stringstream& stream ) const
{
    stream << "<table border = 1>\n";

    stream << "<tr>\n";
    stream << "<th>" << title_.toStdString() << "</th>";
    stream << "</tr>\n";

    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        stream << "<tr>";
        for( int nR = 0; nR < header()->count(); ++nR )
            stream << "<td>" << dataModel_.item( row, nR )->text().toStdString() << "</td>";
        stream << "</tr>\n";
    }
    stream << "</table>\n";
}

inline void SetAutoClear( T_ConnectorVector& v, bool b )
{
    for( T_ConnectorVector::iterator itConnector = v.begin(); itConnector != v.end(); ++itConnector )
        if( *itConnector != 0 )
            ( *itConnector )->SetAutoClear( b );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
bool ADN_ListView::SetCurrentItem( void* pData )
{
    if( pCurData_ == pData )
        return false;

    if( pData == 0 )
        SetAutoClear( vItemConnectors_, true );

    ConnectItem( false );

    pCurData_ = pData;

    ConnectItem( true );

    if( pData == 0 )
        SetAutoClear( vItemConnectors_, false );

    this->blockSignals( true );
    QStandardItem* pItem = this->FindItem( pData );
    selectionModel()->setCurrentIndex( proxyModel_->mapFromSource( dataModel_.indexFromItem( pItem ) ), QItemSelectionModel::ClearAndSelect );
    this->blockSignals( false );

    emit ItemSelected( pData );

    scrollTo( dataModel_.indexFromItem( pItem ) );

    return pData != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
bool ADN_ListView::SetCurrentItem( const QString& itemName )
{
    if( itemName.isEmpty() )
        return false;

    QStandardItem* pItem = FindItem( itemName );

    if( pItem != 0 )
        return SetCurrentItem( static_cast< ADN_ListViewItem* >( pItem )->GetData() );
    else
        return SetCurrentItem( (void*)0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::OnContextMenu
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView::OnContextMenu( const QPoint& /*pt*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillContextMenuWithDefault
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView::FillContextMenuWithDefault( Q3PopupMenu& popupMenu, ADN_ObjectCreator_ABC& objectCreator )
{
    usedByInfos_.clear();
    pObjectCreator_ = &objectCreator;
    popupMenu.insertItem( tr( "Create new" ), this, SLOT( ContextMenuNew() ) );
    if( pCurData_ != 0 )
    {
        popupMenu.insertItem( tr( "Create as copy of" ), this, SLOT( ContextMenuNewCopy() ) );
        popupMenu.insertItem( tr( "Delete" ), this, SLOT( ContextMenuDelete() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillContextMenuWithUsersList
// Created: ABR 2012-01-25
// -----------------------------------------------------------------------------
void ADN_ListView::FillContextMenuWithUsersList( Q3PopupMenu& popupMenu, QString usingName, QString userName, QStringList userList, E_WorkspaceElements targetTab, int subTargetTab /*= -1*/ )
{
    if( pCurData_ != 0 && !userList.isEmpty() )
    {
        ADN_NavigationInfos::UsedBy info;
        info.usersList_.clear();
        info.usingName_ = tr( "< Using: %1 >" ).arg( usingName );
        info.usersList_ = userList;
        info.targetTab_ = targetTab;
        info.subTargetTab_ = subTargetTab;
        usedByInfos_.push_back( info );

        QAction* action = new QAction( tr( "Search for '%1' that use" ).arg( userName.toLower() ), &popupMenu );
        connect( action, SIGNAL( triggered() ), &usedByMapper_, SLOT( map() ) );
        popupMenu.addAction( action );
        usedByMapper_.setMapping( action, static_cast< int >( usedByInfos_.size() ) - 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FormatUsersList
// Created: ABR 2012-01-25
// -----------------------------------------------------------------------------
std::string ADN_ListView::FormatUsersList( const QString& category, const QStringList& usersList ) const
{
    std::string result = "";
    if( usersList.isEmpty() )
        result = tr( "<b>Unused</b>" ).toAscii().constData();
    else
        FillMultiUsersList( category, usersList, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillMultiUsersList
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
void ADN_ListView::FillMultiUsersList( const QString& category, const QStringList& usersList, std::string& result ) const
{
    if( !usersList.isEmpty() )
    {
        if( !result.empty() )
            result += "<br>";
        result += tr( "<nobr><b>Used by the following '%1':</b></nobr>" ).arg( category.toLower() ).toAscii().constData();
        for( QStringList::const_iterator constIterator = usersList.constBegin(); constIterator != usersList.constEnd(); ++constIterator )
            result += "<br><nobr>" + std::string( (*constIterator).toAscii().constData() ) + "</nobr>";
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuSearchElements
// Created: ABR 2012-01-25
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuSearchElements( int id )
{
    emit UsersListRequested( usedByInfos_[ id ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuNew
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuNew()
{
    assert( pObjectCreator_ != 0 );
    FinishCreation( pObjectCreator_->CreateObject() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuNewCopy
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuNewCopy()
{
    assert( pObjectCreator_ != 0 && pCurData_ != 0 );
    FinishCreation( pObjectCreator_->CreateObjectAsCopyOf( pCurData_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FinishCreation
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
void ADN_ListView::FinishCreation( ADN_Ref_ABC* ref )
{
    if( ref == 0 )
    {
        pObjectCreator_ = 0;
        return;
    }

    ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCList->AddItem( ref );

    selectionModel()->setCurrentIndex( dataModel_.indexFromItem( FindItem( ref ) ), QItemSelectionModel::ClearAndSelect );

    pObjectCreator_ = 0;
    static_cast< ADN_MainWindow* >( topLevelWidget() )->ChangeSaveState( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuDelete
// Created: APE 2005-01-27
// -----------------------------------------------------------------------------
bool ADN_ListView::ContextMenuDelete()
{
    if( pCurData_ == 0 || !bDeletionEnabled_ )
        return false;
    ADN_Ref_ABC* pCurrentData = static_cast< ADN_Ref_ABC* >( pCurData_ );

    // Check if the item is multi-referenced, and warn the user if it's the case.
    if( pCurrentData->IsMultiRef() )
    {
        if( !ADN_GuiTools::MultiRefWarning( pCurrentData ) )
            return false;
    }
    else if( !ADN_GuiTools::DeletionWarning() )
        return false;
    // Remove the item from the list.
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentData );
    static_cast< ADN_MainWindow* >( topLevelWidget() )->ChangeSaveState( false );
    delete pCurrentData;
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
bool ADN_ListView::SetCurrentItem()
{
    QModelIndex index = selectionModel()->currentIndex();
    if( index.isValid() )
    {
        QStandardItem* item = dataModel_.GetItemFromIndex( index );
        if( item )
            return SetCurrentItem( static_cast< ADN_ListViewItem* >( item )->GetData() );
    }
    return SetCurrentItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::contextMenuEvent
// Created: JSR 2012-10-01
// -----------------------------------------------------------------------------
void ADN_ListView::contextMenuEvent( QContextMenuEvent* event )
{
    OnContextMenu( event->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ListView::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        gui::RichTreeView::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        gui::RichTreeView::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ListView::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_ListView_ABC* >( pConnector_ )->IsConnected() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::keyReleaseEvent
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
void ADN_ListView::keyReleaseEvent( QKeyEvent* pEvent )
{
    if( pEvent->key() == Qt::Key_Backspace || pEvent->key() == Qt::Key_Delete )
        ContextMenuDelete();
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ComputeNbrPrintPages
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
int ADN_ListView::ComputeNbrPrintPages( const QSize& painterSize ) const
{
    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsRect().width() / painterSize.width() ) );
    int nHeightInPages = static_cast< int >( ceil( ( float ) this->contentsRect().height() / painterSize.height() ) );
    return nWidthInPages * nHeightInPages;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Print
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_ListView::Print( int nPage, QPainter& painter, const QSize& painterSize )
{
    bPrinting_ = true;
    // Ready the table.
    clearSelection();

    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsRect().width() / painterSize.width() ) );

    int nY = static_cast< int >( floor( ( float ) nPage / nWidthInPages ) );
    int nX = nPage % nWidthInPages;

    painter.save();
    painter.translate( -nX * painterSize.width(), -nY * painterSize.height() );
    QTreeView::drawTree( &painter, QRegion( nX * painterSize.width(), nY * painterSize.height(), painterSize.width(), painterSize.height() ) );
    painter.restore();
    bPrinting_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::OnFilterChanged
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_ListView::OnFilterChanged( const QString& filterLine )
{
    if( filterLine.size() > 0 && filterLine[ 0 ] == '<' && filterLine[ filterLine.size() - 1 ] == '>' )
        return;
    filterLine_ = filterLine.lower();
    ApplyFilter( boost::bind( &ADN_ListView::ApplyFilterLine, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::OnFilterChanged
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_ListView::OnFilterChanged( const QStringList& filterList )
{
    filterList_.clear();
    filterList_ = filterList;
    ApplyFilter( boost::bind( &ADN_ListView::ApplyFilterList, this, _1 ) );

    // Select first filtered item.
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        QStandardItem* item = dataModel_.item( i );
        if( item && item->data( gui::StandardModel::FilterRole ).toString() == gui::StandardModel::showValue_ )
        {
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ApplyFilterLine
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
bool ADN_ListView::ApplyFilterLine( ADN_ListViewItem* item )
{
    if( filterLine_.isEmpty() )
        return true;
    if( !item )
        return false;

    QStringList list = filterLine_.split( ' ' );
    bool result = true;
    QString text = item->text().lower();
    for( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
        result = result && text.find( *it ) != -1;
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ApplyFilterList
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
bool ADN_ListView::ApplyFilterList( ADN_ListViewItem* item )
{
    if( filterList_.isEmpty() )
        return true;
    if( !item )
        return false;
    return filterList_.contains( item->text(), Qt::CaseInsensitive ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ApplyFilter
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_ListView::ApplyFilter( boost::function< bool ( ADN_ListViewItem* ) > func )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_ListViewItem* item = static_cast< ADN_ListViewItem* >( dataModel_.item( i ) );
        item->setData( func( item ) ? gui::StandardModel::showValue_ : gui::StandardModel::hideValue_, gui::StandardModel::FilterRole ); // Use HasAnyChildVisible( item, func ) if tree view. Cf HierarchyListView_ABC
    }
}

namespace
{
    int Depth( const QStandardItem* item )
    {
        int depth = 1;
        if( item->rowCount() > 0 )
        {
            int subdepth = 0;
            for( int i = 0; i < item->rowCount(); ++i )
                subdepth = std::max( subdepth, Depth( item->child( i ) ) );
            depth += subdepth;
        }
        return depth;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SaveToXls
// Created: ABR 2012-02-09
// -----------------------------------------------------------------------------
void ADN_ListView::SaveToXls( const QString& path, const QString& sheetName ) const
{
    BasicExcel xls;

    // Compute numer of rows and max depth
    int nbRow = dataModel_.rowCount();
    if( nbRow == 0 )
        return;
    int maxDepth = 0;
    for( int i = 0; i < nbRow; ++i )
        maxDepth = std::max( maxDepth, Depth( dataModel_.item( i ) ) );

    // Excel allow only numeric_limits< USHORT >::max() rows, so if we need more row, we split first level items into multiple sheets.
    // TODO: check if there are not more columns than allowed (255), and check the same for sheets. In both cases -> multiple files.
    if( nbRow < numeric_limits< USHORT >::max() )
    {
        xls.New( 1 );
        SaveToSheet( xls, sheetName.toAscii().constData(), 0, dataModel_.item( 0 ), maxDepth, nbRow );
    }
    else
    {
        xls.New( nbRow );
        int sheet = 0;
        for( int i = 0; i < nbRow; ++i )
            if( dataModel_.item( i )->rowCount() > 0 )
                SaveToSheet( xls, dataModel_.item( i )->text().toAscii().constData(), sheet, dataModel_.item( i )->child( 0 ), maxDepth - 1, nbRow );
    }

    xls.SaveAs( path.toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SaveToSheet
// Created: ABR 2012-02-10
// -----------------------------------------------------------------------------
void ADN_ListView::SaveToSheet( BasicExcel& xls, const char* sheetName, int sheetNumber, QStandardItem* qItem, int maxDepth, int nbRow ) const
{
    // Initialize sheet
    xls.RenameWorksheet( sheetNumber, sheetName );
    BasicExcelWorksheet* sheet = xls.GetWorksheet( sheetNumber );
    if( !sheet )
        return;
    XLSFormatManager fmt_mgr( xls );
    Workbook::Palette& palette = xls.workbook_.palette_;
    palette.SetColor( 0, 200, 200, 200 ); // Header colors

    // Header
    std::vector< int > columnMaxContentSize( header()->count(), 0 );
    int row = 0;
    if( header()->isVisible() )
    {
        CellFormat format( fmt_mgr );
        format.set_borderlines( EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EXCEL_LS_MEDIUM, EGA_BLACK, EGA_BLACK );
        format.set_font( ExcelFont().set_weight(FW_BOLD) );
        format.set_alignment( EXCEL_HALIGN_CENTRED | EXCEL_VALIGN_CENTRED );
        format.set_background( palette.GetColor( 0 ) );

        for( int col = 0; col < header()->count(); ++col )
        {
            int xlsCol = ( col == 0 ) ? 0 : col + maxDepth;
            BasicExcelCell* cell = sheet->Cell( row, xlsCol );
            if( !cell )
                return;
            QString content = dataModel_.headerData( col, Qt::Horizontal ).toString();
            cell->Set( content.toAscii().constData() );
            cell->SetFormat( format );
            if( col == 0 )
            {
                sheet->MergeCells( row, col, 1, static_cast< USHORT >( maxDepth + 1 ) );
                for( int ghostCol = 1; ghostCol < maxDepth + 1; ++ghostCol )
                {
                    BasicExcelCell* ghostCell = sheet->Cell( row, ghostCol );
                    if( !ghostCell )
                        return;
                    ghostCell->SetFormat( format );
                }
            }
            if( columnMaxContentSize[ col ] < content.size() )
                columnMaxContentSize[ col ] = content.size();
        }
        ++row;
    }

    // Fill table from qItem
    RecursiveFillSheetFromItem( qItem, *sheet, fmt_mgr, 0, maxDepth, row, columnMaxContentSize, nbRow );

    // Bottom border
    if( row < std::numeric_limits< USHORT >::max() )
        for( int col = 0; col < header()->count() + maxDepth; ++col )
        {
            BasicExcelCell* cell = sheet->Cell( row, col );
            CellFormat bottom_format( fmt_mgr );
            bottom_format.set_borderlines( EXCEL_LS_NO_LINE, EXCEL_LS_NO_LINE, EXCEL_LS_MEDIUM, EXCEL_LS_NO_LINE, EGA_BLACK, EGA_BLACK );
            cell->SetFormat( bottom_format );
        }

    // Columns width
    static int ghostColumnSize = 1000;
    static int charactereSize = 300;
    static int minimumSize = 1000;
    for( int col = 0; col < header()->count() + maxDepth; ++col )
    {
        USHORT columnSize = 0;
        if( col < maxDepth ) // Ghost column
            columnSize = static_cast< USHORT >( ghostColumnSize );
        else if( col == maxDepth )
            columnSize = static_cast< USHORT >( std::max( columnMaxContentSize[ col - maxDepth ] * charactereSize - maxDepth * ghostColumnSize, minimumSize ) );
        else
            columnSize = static_cast< USHORT >( std::max( columnMaxContentSize[ col - maxDepth ] * charactereSize, minimumSize ) );
        sheet->SetColWidth( col, columnSize );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::RecursiveFillSheetFromItem
// Created: ABR 2012-02-10
// -----------------------------------------------------------------------------
void ADN_ListView::RecursiveFillSheetFromItem( QStandardItem* qItem, BasicExcelWorksheet& sheet, XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize, int nbRow ) const
{
    QStandardItem* p = qItem->parent();
    if( p == 0 )
        p = dataModel_.invisibleRootItem();
    if( !p )
        return;
    for( int i = 0; i < p->rowCount(); ++i )
    {
        QStandardItem* item = p->child( i );
        FillSheetFromItem( item, sheet, fmt_mgr, depth, maxDepth, row, columnMaxContentSize, nbRow );
        if( item->rowCount() > 0 )
            RecursiveFillSheetFromItem( item->child( 0 ), sheet, fmt_mgr, depth + 1, maxDepth, row, columnMaxContentSize, nbRow );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ItemText
// Created: JSR 2012-10-03
// -----------------------------------------------------------------------------
QString ADN_ListView::ItemText( QStandardItem* item, int col ) const
{
    QStandardItem* p = item->parent();
    if( !p )
        p = dataModel_.invisibleRootItem();
    return p->child( item->row(), col )->text();
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillSheetFromItem
// Created: ABR 2012-02-10
// -----------------------------------------------------------------------------
void ADN_ListView::FillSheetFromItem( QStandardItem* qItem, BasicExcelWorksheet& sheet, XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize, int /*nbRow*/ ) const
{
    for( int col = 0; col < header()->count(); ++col )
    {
        int xlsCol = ( col == 0 ) ? depth : col + maxDepth;
        BasicExcelCell* cell = sheet.Cell( row, xlsCol );

        // Content
        QString content = ItemText( qItem, col );
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

        // Border
        CellFormat format( fmt_mgr );
        format.set_borderlines( ( xlsCol == 0 ) ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN, ( col == header()->count() - 1 ) ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN, EXCEL_LS_THIN, EXCEL_LS_THIN, EGA_BLACK, EGA_BLACK );
        if( col == 0 )
        {
            // Update left
            for( int i = 0; i < xlsCol; ++i )
            {
                BasicExcelCell* cell = sheet.Cell( row, i );
                CellFormat ghost_format( fmt_mgr );
                ghost_format.set_borderlines( ( i == 0 ) ? EXCEL_LS_MEDIUM : EXCEL_LS_DOTTED, EXCEL_LS_NO_LINE, EXCEL_LS_NO_LINE, EXCEL_LS_NO_LINE, EGA_BLACK, EGA_BLACK );
                cell->SetFormat( ghost_format );
            }

            // Update right
            for( int i = depth + 1; i < maxDepth + 1; ++i )
            {
                BasicExcelCell* cell = sheet.Cell( row, i );
                CellFormat ghost_format( fmt_mgr );
                ghost_format.set_borderlines( EXCEL_LS_NO_LINE, EXCEL_LS_NO_LINE, EXCEL_LS_THIN, EXCEL_LS_THIN, EGA_BLACK, EGA_BLACK );
                cell->SetFormat( ghost_format );
            }

            // Merge with right
            sheet.MergeCells( row, xlsCol, 1, static_cast< USHORT >( maxDepth + 1 - depth ) );
        }
        cell->SetFormat( format );
    }
    ++row;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::eventFilter
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
bool ADN_ListView::eventFilter( QObject * watched, QEvent * event )
{
    // ???
    if( QMouseEvent* mouseEvent = dynamic_cast< QMouseEvent* >( event ) )
        if( mouseEvent && ( mouseEvent->button() == Qt::XButton1 || mouseEvent->button() == Qt::XButton2 ) )
            return false;
    return RichTreeView::eventFilter( watched, event );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SetGoToOnDoubleClick
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_ListView::SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab /*= -1*/, int col /*= 0*/ )
{
    goToInfo_.targetTab_ = targetTab;
    goToInfo_.subTargetTab_ = subTargetTab;
    goToInfo_.sourceColumn_ = col;
    connect( this, SIGNAL( activated( const QModelIndex& ) ), SLOT( GoToOnDoubleClicked( const QModelIndex& ) ) );
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::GoToOnDoubleClicked
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_ListView::GoToOnDoubleClicked( const QModelIndex& index )
{
    if( !index.isValid() )
        return;
    goToInfo_.targetName_ = dataModel_.itemFromIndex( index )->text();
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}
