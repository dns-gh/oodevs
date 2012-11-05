// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Table_h_
#define __ADN_Table_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_NavigationInfos.h"
#include "ADN_TableDelegate.h"
#include "ADN_Types.h"
#include "ADN_StandardItem.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/VariantPointer.h"

class ADN_Table3 : public QTableView
                 , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors / Destructor
    //@{
            ADN_Table3( const QString& objectName, QWidget* pParent = 0 );
            ADN_Table3( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Table3();
    //@}

public:
    //! @name Operations
    //@{
    // $$$$ ABR 2012-10-23: To Remove when migration finish
    int numRows() const;
    void setNumRows( int numRows );

    virtual void AddRow( int row, void* data );
    void RemoveItem( void* item );

    void SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab = -1, int col = 0 );
    void ActivateRatioColor() {}
    //@}

protected:
    //! @name Helpers
    //@{
    QStandardItem* AddItem( int row, int col, void* parentData, const QString& text, Qt::ItemFlags flags = 0 );
    template< typename T >
    QStandardItem* AddItem( int row, int col, void* parentData, ADN_Type_ABC< T >* data, ADN_StandardItem::E_Type type, Qt::ItemFlags flags = 0 );
    template< typename Enum, int Max >
    QStandardItem* AddItem( int row, int col, void* parentData, ADN_Type_Enum< Enum, Max >* data,  const QStringList& content, Qt::ItemFlags flags = 0 );

    QStandardItem* GetItemFromIndex( const QModelIndex& index ) const;

    virtual void OnContextMenu( const QPoint& pt );
    void* GetSelectedData();
    void* GetDataFromIndex( const QModelIndex& index );
    //@}


    //! @name QTableView overload
    //@{
    virtual bool eventFilter( QObject* watched, QEvent* event );
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize( const QString& objectName );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnGotoRequested( const QModelIndex& index );
    void OnCheckedStateChanged( const QStandardItem& item );
    //@}

signals:
    //! @name Signals
    //@{
    void GoToRequested( const ADN_NavigationInfos::GoTo& );
    //@}

protected:
    //! @name Member data
    //@{
    QStandardItemModel dataModel_;
    ADN_TableDelegate delegate_;
    QSortFilterProxyModel proxyModel_;
    ADN_NavigationInfos::GoTo goToInfo_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddItem
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
inline
QStandardItem* ADN_Table3::AddItem( int row, int col, void* parentData, const QString& text, Qt::ItemFlags flags /* = 0 */ )
{
    if( !parentData )
        return 0;

    ADN_StandardItem* item = new ADN_StandardItem( parentData );
    item->setFlags( Qt::ItemIsEnabled | flags );

    item->setData( text, Qt::EditRole );
    item->setData( text, gui::Roles::DataRole );

    dataModel_.setItem( row, col, item );

    return item;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddItem
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* ADN_Table3::AddItem( int row, int col, void* parentData, ADN_Type_ABC< T >* data, ADN_StandardItem::E_Type type, Qt::ItemFlags flags /* = 0 */ )
{
    if( !data || !parentData )
        return 0;

    // Item creation
    ADN_StandardItem* item = new ADN_StandardItem( parentData, type );
    dataModel_.setItem( row, col, item );

    // Flags
    if( delegate_.IsCheckBox( item->index() ) )
    {
        assert( type == ADN_StandardItem::eBool );
        flags = flags | Qt::ItemIsUserCheckable;
    }
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );

    // Alignment
    if( type != ADN_StandardItem::eString )
        item->setTextAlignment( Qt::AlignRight );

    // Variant
    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( data ) );
    item->setData( *variant, gui::Roles::SafeRole ); // $$$$ ABR 2012-10-25: Use SafeRole to stock the ADN_Type_ABC<> pointer

    // ADN Connection
    item->Connect( data );
    return item;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddItem
// Created: ABR 2012-10-26
// -----------------------------------------------------------------------------
template< typename Enum, int Max >
inline
QStandardItem* ADN_Table3::AddItem( int row, int col, void* parentData, ADN_Type_Enum< Enum, Max >* data, const QStringList& content, Qt::ItemFlags flags /*= 0*/ )
{
    if( !data || !parentData )
        return 0;

    // Item creation
    ADN_StandardItem* item = new ADN_StandardItem( parentData, ADN_StandardItem::eEnum );
    dataModel_.setItem( row, col, item );

    // Flags
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );

    // Variant
    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( data ) );
    item->setData( *variant, gui::Roles::SafeRole ); // $$$$ ABR 2012-10-25: Use SafeRole to stock the ADN_Type_ABC<> pointer

    // ADN Connection
    item->Connect( data, &content );
    return item;
}





























//*****************************************************************************
// Created: JDY 03-07-07
//*****************************************************************************
class ADN_Table : public Q3Table, public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_Table( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Table();

    void  EnableRefreshing( bool b );
    void  StopEditing();
    void* GetCurrentData();
    void  setEnabled( bool b );

    void AdjustColumns( int nMinWidth = -1 );
    void SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab = -1, int col = 0 );

    int ComputeNbrPrintPages( const QSize& painterSize ) const;
    void Print( int nPage, QPainter& painter, const QSize& painterSize );
    void SaveToXls( const QString& path, const QString& sheetName ) const;

    void AddBoldGridRow( int nIndex );
    void AddBoldGridCol( int nIndex );

    // SBO 2005-09-01 : was protected
    virtual void sortColumn( int nCol, bool bAscending = true, bool wholerows = false );
    virtual bool event(QEvent *event);

protected:
    virtual void drawContents ( QPainter * p, int cx, int cy, int cw, int ch );

    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected );
    virtual void paintEmptyArea ( QPainter * p, int cx, int cy, int cw, int ch );
    virtual bool eventFilter( QObject * watched, QEvent * event );

    virtual QWidget* createEditor( int nRow, int nCol, bool bInitFromCell ) const;

protected slots:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    void UpdateEnableState();
    QString GetToolTips( int nRow, int nCol ) const;

    virtual void doValueChanged( int row, int col );
    void GoToOnDoubleClicked( int row, int col, int button, const QPoint & mousePos );

signals:
    void GoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );

protected:
    typedef std::set<int>         T_IndexSet;
    typedef T_IndexSet::iterator IT_IndexSet;

    T_IndexSet                  vBoldGridRowIndexes_;
    T_IndexSet                  vBoldGridColIndexes_;

    bool                        bRefreshingEnabled_;
    bool                        bPrinting_;
    ADN_NavigationInfos::GoTo   goToInfo_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Table::EnableRefreshing
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
inline
void ADN_Table::EnableRefreshing( bool b )
{
    bRefreshingEnabled_ = b;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddBoldGridRow
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
inline
void ADN_Table::AddBoldGridRow( int nIndex )
{
    vBoldGridRowIndexes_.insert( nIndex );
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddBoldGridCol
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
inline
void ADN_Table::AddBoldGridCol( int nIndex )
{
    vBoldGridColIndexes_.insert( nIndex );
}

class ADN_Table2 : public ADN_Table
{
public:
    explicit ADN_Table2( QWidget* pParent, const char* szName  ) : ADN_Table( pParent, szName ) {}
    virtual ~ADN_Table2() {};

    //! @name Qt reimplementation.
    //@{
    QString tr( const char* s, const char* c = 0 )
    {
        if( qApp )
            return qApp->translate( this->name(), s, c, QApplication::DefaultCodec );
        else
            return QString::fromLatin1( s );
    }
    //@}
};

#endif // __ADN_Table_h_