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
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Languages_Data.h"
#include "ADN_Languages_GUI.h"
#include "ADN_ListViewToolTip.h"
#include "ADN_MultiRefWarningDialog.h"
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_StandardItem.h"
#include "ADN_Tools.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/LanguageController.h"
#include <boost/bind.hpp>
#include <excel/ExcelFormat.h>

using namespace ExcelFormat;

namespace
{
    bool MultiRefWarning( ADN_Ref_ABC* data /* = 0 */ )
    {
        ADN_Workspace::T_UsingElements elementsToDelete;
        ADN_Workspace::T_UsingElements usingElements;
        if( data )
        {
            elementsToDelete = ADN_Workspace::GetWorkspace().GetElementThatWillBeDeleted( data );
            usingElements = ADN_Workspace::GetWorkspace().GetElementThatUse( data );
        }
        if( elementsToDelete.empty() && usingElements.empty() )
            return QMessageBox::Ok == QMessageBox::warning( 0,
            qApp->translate( "ADN_Tools", "Multi references" ),
            qApp->translate( "ADN_Tools", "This item is referenced by at least one other item.\nClick \"Ok\" to destroy it and all its references." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );

        return ADN_MultiRefWarningDialog( 0, elementsToDelete, usingElements ).exec() == QMessageBox::Ok;
    }

    bool DeletionWarning()
    {
        int nResult = QMessageBox::warning( 0,
            qApp->translate( "ADN_Tools", "Delete?" ),
            qApp->translate( "ADN_Tools", "Really delete this item?\nClick \"Ok\" to delete it." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );

        return nResult == QMessageBox::Ok;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::ADN_ListView( QWidget* pParent, const char* szName, const QString title /*= ""*/ ) // flags à virer
    : gui::RichTreeView( szName, pParent )
    , ADN_Gfx_ABC       ()
    , pCurData_         ( 0 )
    , title_            ( title )
    , pObjectCreator_   ( 0 )
    , bDeletionEnabled_ ( false )
    , bDeletionWarning_ ( true )
    , bPrinting_        ( false )
    , usedByMapper_     ( this )
{
    setRootIsDecorated( false );
    if( !title.isEmpty() )
        dataModel_.setHorizontalHeaderLabels( QStringList( title ) );
    viewport()->installEventFilter( new ADN_ListViewToolTip( viewport(), *this ) );

    proxyModel_->sort( 0 );

    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( SetCurrentItem() ) );
    connect( &usedByMapper_, SIGNAL( mapped( int ) ), this, SLOT( ContextMenuSearchElements( int ) ) );
    ADN_Workspace::GetWorkspace().GetLanguageController().Register( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::~ADN_ListView()
{
    ADN_Workspace::GetWorkspace().GetLanguageController().Unregister( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::ItemAt
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_StandardItem* ADN_ListView::ItemAt( int i )
{
    assert( i < dataModel_.rowCount() );
    return static_cast< ADN_StandardItem* >( dataModel_.item( i ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_StandardItem* ADN_ListView::FindItem( void* data )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_StandardItem* pCurr = static_cast< ADN_StandardItem* >( dataModel_.item( i ) );
        if( pCurr->GetData() == data )
            return pCurr;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
ADN_StandardItem* ADN_ListView::FindItem( const QString& itemName  )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_StandardItem* pCurr = static_cast< ADN_StandardItem* >( dataModel_.item( i ) );
        if( pCurr->text() == itemName )
            return pCurr;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::InsertItem
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::InsertItem( ADN_StandardItem* item )
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
void ADN_ListView::TakeItem( ADN_StandardItem* item )
{
    const QModelIndex index = dataModel_.indexFromItem( item );
    dataModel_.removeRow( index.row(), index.parent() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::MoveItem
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::MoveItem( ADN_StandardItem* src, ADN_StandardItem* dest )
{
    dataModel_.insertRow( dest->row() + 1, dataModel_.takeRow( src->row() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::MoveItemAbove
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::MoveItemAbove( ADN_StandardItem* src, ADN_StandardItem* dest )
{
    dataModel_.insertRow( dest->row(), dataModel_.takeRow( src->row() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Swap
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_ListView::Swap( ADN_StandardItem* src, ADN_StandardItem* dest )
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
    dataModel_.removeRows( 0, dataModel_.rowCount() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ClearFilters
// Created: JSR 2012-12-14
// -----------------------------------------------------------------------------
void ADN_ListView::ClearFilters()
{
    emit PurgeLineEdit();
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

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
bool ADN_ListView::SetCurrentItem( void* pData )
{
    if( pCurData_ == pData )
        return false;

    if( pData == 0 )
        ADN_Tools::SetAutoClear( vItemConnectors_, true );

    bool wasModified = topLevelWidget()->isWindowModified(); // $$$$ ABR 2012-11-16: Hack to avoid window modifier to appear when connecting an item (because of weapon graph and unit icon)

    ConnectItem( false );

    pCurData_ = pData;

    ConnectItem( true );

    ADN_Workspace::GetWorkspace().SetMainWindowModified( wasModified );

    if( pData == 0 )
        ADN_Tools::SetAutoClear( vItemConnectors_, false );

    bool wasBlocked = this->blockSignals( true );
    QStandardItem* pItem = this->FindItem( pData );
    selectionModel()->setCurrentIndex( proxyModel_->mapFromSource( dataModel_.indexFromItem( pItem ) ), QItemSelectionModel::ClearAndSelect );
    this->blockSignals( wasBlocked );

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
        return SetCurrentItem( static_cast< ADN_StandardItem* >( pItem )->GetData() );
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
    pObjectCreator_ = 0;
    if( !ADN_Workspace::GetWorkspace().GetLanguages().GetData().IsCurrentMaster() )
        return;
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
        result = tr( "<b>Unused</b>" ).toStdString();
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
        result += tr( "<nobr><b>Used by the following '%1':</b></nobr>" ).arg( category.toLower() ).toStdString();
        for( QStringList::const_iterator constIterator = usersList.constBegin(); constIterator != usersList.constEnd(); ++constIterator )
            result += "<br><nobr>" + std::string( (*constIterator).toStdString() ) + "</nobr>";
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

    ADN_Connector_Vector_ABC& pCList = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCList.AddItem( ref );

    selectionModel()->setCurrentIndex( dataModel_.indexFromItem( FindItem( ref ) ), QItemSelectionModel::ClearAndSelect );

    pObjectCreator_ = 0;
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
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
        if( !MultiRefWarning( pCurrentData ) )
            return false;
    }
    else if( !DeletionWarning() )
        return false;
    // Remove the item from the list.
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurrentData );
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
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
            return SetCurrentItem( static_cast< ADN_StandardItem* >( item )->GetData() );
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
int ADN_ListView::ComputeNbrPrintPages( const QSize& painterSize )
{
    expandAll();
    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsRect().width() / painterSize.width() ) );
    int nHeightInPages = static_cast< int >( ceil( ( float ) this->contentsRect().height() / painterSize.height() ) );
    collapseAll();
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
    expandAll();
    drawTree( &painter, QRegion( nX * painterSize.width(), nY * painterSize.height(), painterSize.width(), painterSize.height() ) );
    collapseAll();
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
        if( item && item->data( gui::Roles::FilterRole ).toString() == gui::StandardModel::showValue_ )
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
bool ADN_ListView::ApplyFilterLine( ADN_StandardItem* item )
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
bool ADN_ListView::ApplyFilterList( ADN_StandardItem* item )
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
void ADN_ListView::ApplyFilter( std::function< bool( ADN_StandardItem* ) > func )
{
    const int rowCount = dataModel_.rowCount();
    for( int i = 0; i < rowCount; ++i )
    {
        ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( i ) );
        item->setData( func( item ) ? gui::StandardModel::showValue_ : gui::StandardModel::hideValue_, gui::Roles::FilterRole );
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
void ADN_ListView::SaveToXls( const tools::Path& path, const QString& sheetName ) const
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
        SaveToSheet( xls, sheetName.toStdString().c_str(), 0, dataModel_.item( 0 ), maxDepth, nbRow );
    }
    else
    {
        xls.New( nbRow );
        int sheet = 0;
        for( int i = 0; i < nbRow; ++i )
            if( dataModel_.item( i )->rowCount() > 0 )
                SaveToSheet( xls, dataModel_.item( i )->text().toStdString().c_str(), sheet, dataModel_.item( i )->child( 0 ), maxDepth - 1, nbRow );
    }

    xls.SaveAs( path.ToUnicode().c_str() );
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
            cell->Set( content.toStdWString().c_str() );
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
// Name: ADN_Table::mousePressEvent
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_ListView::mousePressEvent( QMouseEvent* event )
{
    if( event && ( event->button() == Qt::XButton1 || event->button() == Qt::XButton2 ) )
        event->ignore();
    else
        gui::RichTreeView::mousePressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SetGoToOnDoubleClick
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_ListView::SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab /*= -1*/, int col /* = 0 */ )
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
    QStandardItem* item = dataModel_.GetItemFromIndex( index );
    if( !item )
        return;
    goToInfo_.targetName_ = item->text();
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Warn
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
void ADN_ListView::Warn( ADN_ErrorStatus /* errorStatus = eNoError */, const QString& /* = "" */ )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row ) ) )
            if( ADN_Ref_ABC* parentData = reinterpret_cast< ADN_Ref_ABC* >( item->GetData() ) )
                item->Warn( parentData->GetErrorStatus() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::RemoveCurrentElement
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
void ADN_ListView::RemoveCurrentElement()
{
    void* data = GetCurrentData();
    if( data )
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( data );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::OnLanguageChanged
// Created: ABR 2013-07-17
// -----------------------------------------------------------------------------
void ADN_ListView::OnLanguageChanged()
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
        if( ADN_StandardItem* item = static_cast< ADN_StandardItem* >( dataModel_.item( row ) ) )
            if( ADN_Ref_ABC* parentData = reinterpret_cast< ADN_Ref_ABC* >( item->GetData() ) )
            {
                parentData->CheckValidity();
                item->Warn( parentData->GetErrorStatus() );
            }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Sort
// Created: ABR 2013-10-11
// -----------------------------------------------------------------------------
void ADN_ListView::Sort( int column /*= 0*/, Qt::SortOrder order /*= Qt::AscendingOrder*/ )
{
    proxyModel_->sort( column, order );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::Title
// Created: ABR 2014-03-31
// -----------------------------------------------------------------------------
const QString& ADN_ListView::Title() const
{
    return title_;
}
