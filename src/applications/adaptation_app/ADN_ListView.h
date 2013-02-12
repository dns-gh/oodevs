//*****************************************************************************
//
// $Created: JDY 03-06-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView.h $
// $Author: Ape $
// $Modtime: 25/04/05 10:43 $
// $Revision: 7 $
// $Workfile: ADN_ListView.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_h_
#define __ADN_ListView_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_ABC.h"
#include "ADN_NavigationInfos.h"
#include "clients_gui/RichTreeView.h"
#include <boost/function.hpp>

class ADN_Connector_ListView_ABC;
class ADN_StandardItem;
class ADN_ObjectCreator_ABC;

namespace YExcel
{
    class BasicExcel;
    class BasicExcelWorksheet;
}
namespace ExcelFormat
{
    struct XLSFormatManager;
}

// =============================================================================
/** @class  ADN_ListView
    @brief  ADN_ListView
*/
// Created: AGN 2003-11-18
// =============================================================================
class ADN_ListView : public gui::RichTreeView
                   , public ADN_Gfx_ABC
{
    Q_OBJECT
    friend ADN_Connector_ListView_ABC;

public:
    explicit ADN_ListView( QWidget* pParent, const char* szName, const QString title = "" );
    virtual ~ADN_ListView();

    ADN_StandardItem*   ItemAt( int i );
    ADN_StandardItem*   FindItem( void* pData );
    ADN_StandardItem*   FindItem( const QString& itemName );
    void*               GetCurrentData();
    void*               GetData( const QPoint& pt );
    void                SetItemConnectors( const T_ConnectorVector& v );

    void InsertItem( ADN_StandardItem* item );
    void InsertItems( const QList< QStandardItem* >& items );
    void TakeItem( ADN_StandardItem* item );
    void MoveItem( ADN_StandardItem* src, ADN_StandardItem* dest );
    void MoveItemAbove( ADN_StandardItem* src, ADN_StandardItem* dest );
    void Swap( ADN_StandardItem* src, ADN_StandardItem* dest );
    int ChildCount() const;
    void Clear();
    void ClearFilters();
    void CreateTableFrom( std::stringstream& stream ) const;
    void RemoveCurrentElement();

    void setEnabled( bool b );

    void SetDeletionEnabled( bool enable, bool warning = true );
    void SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab = -1, int col = 0 );

    int ComputeNbrPrintPages( const QSize& painterSize );
    void Print( int nPage, QPainter& painter, const QSize& painterSize );

    virtual std::string GetToolTipFor( const QModelIndex& index );
    void SaveToXls( const QString& path, const QString& sheetName ) const;

public slots:
    bool SetCurrentItem( void* pData );
    bool SetCurrentItem( const QString& itemName );
    void OnFilterChanged( const QString& );
    void OnFilterChanged( const QStringList& );

protected:
    virtual void ConnectItem( bool /*bConnect*/ ){}// = 0;

    void keyReleaseEvent( QKeyEvent* pEvent );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void contextMenuEvent( QContextMenuEvent* event );

    virtual void OnContextMenu( const QPoint& pt );
    void FillContextMenuWithDefault( Q3PopupMenu& popupMenu, ADN_ObjectCreator_ABC& objectCreator );
    void FillContextMenuWithUsersList( Q3PopupMenu& popupMenu, QString usingName, QString userName, QStringList userList, E_WorkspaceElements targetTab, int subTargetTab = -1 );
    std::string FormatUsersList( const QString& category, const QStringList& usersList ) const;
    void FillMultiUsersList( const QString& category, const QStringList& usersList, std::string& result ) const;

private:
    virtual QStringList MimeTypes() const { return QStringList(); }
    virtual void Drop( const QString& /*mimeType*/, void* /*data*/, QStandardItem& /*target*/ ) {}
    virtual QMimeData* MimeData( const QModelIndexList& /*indexes*/, bool& /*overriden*/ ) const { return 0; }
    bool ApplyFilterLine( ADN_StandardItem* item );
    bool ApplyFilterList( ADN_StandardItem* item );
    void ApplyFilter( boost::function< bool ( ADN_StandardItem* ) > func );
    void SaveToSheet( YExcel::BasicExcel& xls, const char* sheetName, int sheetNumber, QStandardItem* item, int maxDepth, int nbRow ) const;
    void RecursiveFillSheetFromItem( QStandardItem* qItem, YExcel::BasicExcelWorksheet& sheet, ExcelFormat::XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize, int nbRow ) const;
    void FillSheetFromItem( QStandardItem* qItem, YExcel::BasicExcelWorksheet& sheet, ExcelFormat::XLSFormatManager& fmt_mgr, int depth, int maxDepth, int& row, std::vector< int >& columnMaxContentSize, int nbRow ) const;
    void FinishCreation( ADN_Ref_ABC* ref );
    QString ItemText( QStandardItem* item, int col ) const;

protected slots:
    virtual void ContextMenuNew();
    virtual void ContextMenuNewCopy();
    virtual bool ContextMenuDelete();
    virtual void ContextMenuSearchElements( int id );

    void GoToOnDoubleClicked( const QModelIndex& index );
    bool SetCurrentItem();
    void UpdateEnableState();

    void Warn( ADN_ErrorStatus errorStatus, const QString& errorMsg );

signals:
    void ItemSelected( void* pData );
    void UsersListRequested( const ADN_NavigationInfos::UsedBy& usedByInfo );
    void GoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );
    void PurgeLineEdit();

protected:
    void*                       pCurData_;
    const QString               title_;
    T_ConnectorVector           vItemConnectors_;
    ADN_ObjectCreator_ABC*      pObjectCreator_;
    bool                        bDeletionEnabled_;
    bool                        bDeletionWarning_;
    bool                        bPrinting_;
    QRect                       toolTipRect_;
    QSignalMapper                               usedByMapper_;
    std::vector< ADN_NavigationInfos::UsedBy >  usedByInfos_;
    ADN_NavigationInfos::GoTo                   goToInfo_;
    QString                                     filterLine_;
    QStringList                                 filterList_;
};

//-----------------------------------------------------------------------------
// Name: ADN_ListView::GetCurrentData
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
inline
void* ADN_ListView::GetCurrentData()
{
    return pCurData_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::GetData
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
inline
void* ADN_ListView::GetData( const QPoint& pt )
{
    if( !viewport() )
        return 0;
    QModelIndex index = indexAt( viewport()->mapFromGlobal( pt ) );
    if( index.isValid() )
        return GetCurrentData();
    return 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetItemConnectors
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
inline
void ADN_ListView::SetItemConnectors( const T_ConnectorVector& v )
{
    vItemConnectors_ = v;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::SetDeletionEnabled
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
inline
void ADN_ListView::SetDeletionEnabled( bool enabled, bool warning )
{
    bDeletionEnabled_ = enabled;
    bDeletionWarning_ = warning;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
inline
std::string ADN_ListView::GetToolTipFor( const QModelIndex& /*index*/ )
{
    return std::string( "" );
}

#endif // __ADN_ListView_h_
