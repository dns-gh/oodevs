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
#include "ADN_UsedByInfos.h"
#include <boost/function.hpp>

class ADN_ListViewItem;
class ADN_Connector_ListView_ABC;
class ADN_ObjectCreator_ABC;

// =============================================================================
/** @class  ADN_ListView
    @brief  ADN_ListView
*/
// Created: AGN 2003-11-18
// =============================================================================
class ADN_ListView : public Q3ListView, public ADN_Gfx_ABC
{
    Q_OBJECT
    friend ADN_Connector_ListView_ABC;

public:
    explicit ADN_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView();

    ADN_ListViewItem*     ItemAt( int i );
    ADN_ListViewItem*     FindItem( void* pData );
    int                   FindNdx( void* pData );
    void*                 GetCurrentData();
    void                  SetItemConnectors( const T_ConnectorVector& v );

    void setEnabled( bool b );

    void SetDeletionEnabled( bool enable, bool warning = true );

    int ComputeNbrPrintPages( const QSize& painterSize ) const;
    void Print( int nPage, QPainter& painter, const QSize& painterSize );

    virtual std::string GetToolTipFor( Q3ListViewItem& item );

public slots:
    void SetCurrentItem( void* pData );
    void OnFilterChanged( const QString& );
    void OnFilterChanged( const QStringList& );

protected:
    virtual void ConnectItem( bool /*bConnect*/ ){}// = 0;

    void keyReleaseEvent( QKeyEvent* pEvent );

    virtual void OnContextMenu( const QPoint& pt );
    void FillContextMenuWithDefault( Q3PopupMenu& popupMenu, ADN_ObjectCreator_ABC& objectCreator );
    void FillContextMenuWithUsersList( Q3PopupMenu& popupMenu, QString usingName, QStringList userList, E_WorkspaceElements targetTab, int subTargetTab = -1 );
    std::string FormatUsersList( QStringList usersList );

private:
    bool ApplyFilterLine( ADN_ListViewItem* item );
    bool ApplyFilterList( ADN_ListViewItem* item );
    void ApplyFilter( boost::function< bool ( ADN_ListViewItem* ) > func );

private slots:
    virtual void ContextMenuNew();
    virtual void ContextMenuDelete();
    virtual void ContextMenuSearchElements();

    void SetCurrentItem( Q3ListViewItem* pItem );
    void OnContextMenuRequested( Q3ListViewItem* pItem, const QPoint& pt, int nCol );
    void UpdateEnableState();

signals:
    void ItemSelected( void* pData );
    void UsersListRequested( const ADN_UsedByInfos& usedByInfo );

protected:
    void*                   pCurData_;
    T_ConnectorVector       vItemConnectors_;
    ADN_ObjectCreator_ABC*  pObjectCreator_;
    bool                    bDeletionEnabled_;
    bool                    bDeletionWarning_;
    bool                    bPrinting_;
    QRect                   toolTipRect_;
    ADN_UsedByInfos         usedByInfo_;
    QString                 filterLine_;
    QStringList             filterList_;
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
std::string ADN_ListView::GetToolTipFor( Q3ListViewItem& /*item*/ )
{
    return std::string( "" );
}

#endif // __ADN_ListView_h_