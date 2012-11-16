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
#include "excel/ExcelFormat.h"

using namespace ExcelFormat;

//-----------------------------------------------------------------------------
// Name: ADN_ListView constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::ADN_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : Q3ListView         ( pParent, szName, f )
    , ADN_Gfx_ABC       ()
    , pCurData_         ( 0 )
    , pObjectCreator_   ( 0 )
    , bDeletionEnabled_ ( false )
    , bDeletionWarning_ ( true )
    , bPrinting_        ( false )
    , usedByMapper_     ( this )
{
    viewport()->installEventFilter( new ADN_ListViewToolTip( viewport(), *this ) );

    connect( this, SIGNAL( selectionChanged( Q3ListViewItem* ) ), this, SLOT( SetCurrentItem( Q3ListViewItem* ) ) );
    connect( this, SIGNAL( currentChanged( Q3ListViewItem* ) ), this, SLOT( SetCurrentItem( Q3ListViewItem * ) ) );
    connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( Q3ListViewItem*, const QPoint &, int ) ) );

    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );

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
// Name: ADN_ListView::FindNdx
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
int ADN_ListView::FindNdx(void *data)
{
    int ndx=0;
    Q3ListViewItemIterator it( this );
    while ( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr=(ADN_ListViewItem*)it.current();
        if( pCurr->GetData()==data )
            return ndx;
        ++it;
        ++ndx;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::ItemAt
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::ItemAt(int i)
{
    int ndx=0;
    Q3ListViewItemIterator it( this );
    while ( it.current() != 0 && ndx < i )
    {
        ++it;
        ++ndx;
    }
    return (ADN_ListViewItem*)it.current();
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem(void* data)
{
    Q3ListViewItemIterator it( this );
    while ( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr=(ADN_ListViewItem*)it.current();
        if( pCurr->GetData()==data )
            return pCurr;
        ++it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem( const QString& itemName, int col /* = 0 */ )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr=(ADN_ListViewItem*)it.current();
        if( pCurr->text( col ) == itemName )
            return pCurr;
        ++it;
    }
    return 0;
}


inline void SetAutoClear(T_ConnectorVector& v,bool b)
{
    for(  T_ConnectorVector::iterator itConnector=v.begin();itConnector!=v.end();++itConnector)
        if( *itConnector != 0 )
            (*itConnector)->SetAutoClear(b);
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

    bool wasModified = topLevelWidget()->isWindowModified(); // $$$$ ABR 2012-11-16: Hack to avoid window modifier to appear when connecting an item (because of weapon graph and unit icon)

    ConnectItem( false );

    pCurData_ = pData;

    ConnectItem( true );

    topLevelWidget()->setWindowModified( wasModified );

    if( pData == 0 )
        SetAutoClear( vItemConnectors_, false );

    this->blockSignals( true );
    Q3ListViewItem* pItem = this->FindItem( pData );
    this->setSelected( pItem, true );
    this->blockSignals( false );

    emit ItemSelected( pData );

    ensureItemVisible( pItem );

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

    Q3ListViewItem* pItem = FindItem( firstChild(), itemName );

    if( pItem != 0 )
        return SetCurrentItem( static_cast<ADN_ListViewItem*>( pItem )->GetData() );
    else
        return SetCurrentItem( (void*)0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem( Q3ListViewItem* qItem, const QString& itemName )
{
    ADN_ListViewItem* result = 0;
    for( Q3ListViewItem* item = qItem; item != 0 && result == 0; item = item->nextSibling() )
    {
        if( item->text( 0 ) == itemName )
            result = static_cast< ADN_ListViewItem* >( item );
        else
            result = FindItem( item->firstChild(), itemName );
    }
    return result;
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
        usedByMapper_.setMapping( action, usedByInfos_.size() - 1 );
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

    setCurrentItem( FindItem( ref ) );

    pObjectCreator_ = 0;
    topLevelWidget()->setWindowModified( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuDelete
// Created: APE 2005-01-27
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuDelete()
{
    if( pCurData_ == 0 || !bDeletionEnabled_ )
        return;

    ADN_Ref_ABC* pCurrentData = static_cast< ADN_Ref_ABC* >( pCurData_ );

    // Check if the item is multi-referenced, and warn the user if it's the case.
    if( pCurrentData->IsMultiRef() )
    {
        if( !ADN_GuiTools::MultiRefWarning( pCurrentData ) )
            return;
    }
    else if( !ADN_GuiTools::DeletionWarning() )
        return;
    // Remove the item from the list.
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentData );
    topLevelWidget()->setWindowModified( true );
    delete pCurrentData;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
bool ADN_ListView::SetCurrentItem( Q3ListViewItem* pItem )
{
    if( pItem != 0 && pItem->isVisible() )
        return SetCurrentItem( static_cast<ADN_ListViewItem*>( pItem )->GetData() );
    else
        return SetCurrentItem( (void*)0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::OnContextMenuRequested
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView::OnContextMenuRequested( Q3ListViewItem* /*pItem*/, const QPoint& pt, int /*nCol*/ )
{
    OnContextMenu( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ListView::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        Q3ListView::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        Q3ListView::setEnabled( b );
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
    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsWidth() / painterSize.width() ) );
    int nHeightInPages = static_cast< int >( ceil( ( float ) this->contentsHeight() / painterSize.height() ) );
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

    int nWidthInPages = static_cast< int >( ceil( ( float ) this->contentsWidth() / painterSize.width() ) );

    int nY = static_cast< int >( floor( ( float ) nPage / nWidthInPages ) );
    int nX = nPage % nWidthInPages;

    painter.save();
    painter.translate( -nX * painterSize.width(), -nY * painterSize.height() );
    Q3ListView::drawContentsOffset( &painter, 0, 0, nX * painterSize.width(), nY * painterSize.height(), painterSize.width(), painterSize.height() );
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
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
    {
        ADN_ListViewItem* item = static_cast< ADN_ListViewItem* >( it.current() );
        if( item->isVisible() )
        {
            setSelected( item, true );
            break;
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
    QString text = item->text( 0 ).lower();
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
    return filterList_.contains( item->text( 0 ), Qt::CaseInsensitive ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ApplyFilter
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_ListView::ApplyFilter( boost::function< bool ( ADN_ListViewItem* ) > func )
{
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
    {
        ADN_ListViewItem* item = static_cast< ADN_ListViewItem* >( it.current() );
        item->setVisible( func( item ) ); // Use HasAnyChildVisible( item, func ) if tree view. Cf HierarchyListView_ABC
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
    int nbRow = 0;
    int maxDepth = 0;
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it, ++nbRow )
        maxDepth = std::max( maxDepth, it.current()->depth() );

    // Excel allow only numeric_limits< USHORT >::max() rows, so if we need more row, we split first level items into multiple sheets.
    // TODO: check if there are not more columns than allowed (255), and check the same for sheets. In both cases -> multiple files.
    if( nbRow < numeric_limits< USHORT >::max() )
    {
        xls.New( 1 );
        SaveToSheet( xls, sheetName.toAscii().constData(), 0, firstChild(), maxDepth );
    }
    else
    {
        int sheet = 0;
        for( Q3ListViewItem* item = firstChild(); item; item = item->nextSibling(), ++sheet );
        xls.New( sheet );
        sheet = 0;
        for( Q3ListViewItem* item = firstChild(); item; item = item->nextSibling(), ++sheet )
            SaveToSheet( xls, item->text( 0 ).toAscii().constData(), sheet, item->firstChild(), maxDepth - 1 );
    }

    xls.SaveAs( path.toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SaveToSheet
// Created: ABR 2012-02-10
// -----------------------------------------------------------------------------
void ADN_ListView::SaveToSheet( BasicExcel& xls, const char* sheetName, int sheetNumber, Q3ListViewItem* qItem, int maxDepth ) const
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
    std::vector< int > columnMaxContentSize( columns(), 0 );
    int row = 0;
    if( header()->isVisible() )
    {
        assert( header()->count() == columns() );
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
            QString content = header()->label( col );
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
    RecursiveFillSheetFromItem( qItem, *sheet, fmt_mgr, 0, maxDepth, row, columnMaxContentSize );

    // Bottom border
    if( row < std::numeric_limits< USHORT >::max() )
        for( int col = 0; col < columns() + maxDepth; ++col )
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
    for( int col = 0; col < columns() + maxDepth; ++col )
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
void ADN_ListView::RecursiveFillSheetFromItem( Q3ListViewItem* qItem, BasicExcelWorksheet& sheet, XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize ) const
{
    for( Q3ListViewItem* item = qItem; item; item = item->nextSibling() )
    {
        FillSheetFromItem( item, sheet, fmt_mgr, depth, maxDepth, row, columnMaxContentSize );
        RecursiveFillSheetFromItem( item->firstChild(), sheet, fmt_mgr, depth + 1, maxDepth, row, columnMaxContentSize );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillSheetFromItem
// Created: ABR 2012-02-10
// -----------------------------------------------------------------------------
void ADN_ListView::FillSheetFromItem( Q3ListViewItem* qItem, BasicExcelWorksheet& sheet, XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize ) const
{
    for( int col = 0; col < columns(); ++col )
    {
        int xlsCol = ( col == 0 ) ? depth : col + maxDepth;
        BasicExcelCell* cell = sheet.Cell( row, xlsCol );

        // Content
        QString content = qItem->text( col );
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
        format.set_borderlines( ( xlsCol == 0 ) ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN, ( col == columns() - 1 ) ? EXCEL_LS_MEDIUM : EXCEL_LS_THIN, EXCEL_LS_THIN, EXCEL_LS_THIN, EGA_BLACK, EGA_BLACK );
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
    if( QMouseEvent* mouseEvent = dynamic_cast< QMouseEvent* >( event ) )
        if( mouseEvent && ( mouseEvent->button() == Qt::XButton1 || mouseEvent->button() == Qt::XButton2 ) )
            return false;
    return Q3ListView::eventFilter( watched, event );
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
    connect( this, SIGNAL( doubleClicked( Q3ListViewItem* ) ), SLOT( GoToOnDoubleClicked( Q3ListViewItem* ) ) );
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::GoToOnDoubleClicked
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_ListView::GoToOnDoubleClicked( Q3ListViewItem* pItem )
{
    if( !pItem )
        return;
    goToInfo_.targetName_ = pItem->text( goToInfo_.sourceColumn_ );
    assert( goToInfo_.targetTab_ != eNbrWorkspaceElements );
    emit( GoToRequested( goToInfo_ ) );
}
