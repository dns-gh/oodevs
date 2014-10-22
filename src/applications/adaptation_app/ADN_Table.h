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

#include "ADN_Table_ABC.h"
#include "ADN_NavigationInfos.h"
#include "ADN_TableDelegate.h"
#include "ADN_Types.h"
#include "ADN_StandardItem.h"
#include "tools/LanguageObserver_ABC.h"

class ADN_Table : public QTableView
                , public ADN_Table_ABC
                , public tools::Observer_ABC
                , public tools::LanguageObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors / Destructor
    //@{
             ADN_Table( const QString& objectName, QWidget* pParent = 0 );
             ADN_Table( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent = 0 );
             ADN_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Table();
    //@}

public:
    //! @name Operations
    //@{
    int numRows() const;
    virtual void setNumRows( int numRows );

    virtual void AddRow( int row, void* data );
    virtual void RemoveItem( void* item );
    virtual void RemoveCurrentElement();

    void SetGoToOnDoubleClick( E_WorkspaceElements targetTab, int subTargetTab = -1, int col = 0 );
    void Sort( int column = 0, Qt::SortOrder order = Qt::AscendingOrder );

    QStandardItem* GetItem( int row, int col ) const;
    QStandardItem* GetItemFromIndex( const QModelIndex& index ) const;
    void* GetData( const QPoint& pt );

    void AddBoldGridRow( int nIndex );
    void AddBoldGridCol( int nIndex );

    int ComputeNbrPrintPages( const QSize& painterSize ) const;
    void Print( int nPage, QPainter& painter, const QSize& painterSize );
    void SaveToXls( const tools::Path& path, const QString& sheetName ) const;

    template< typename T >
    T* CreateNewElement();

    virtual void CheckValidity( int row = -1, int col = -1 );

    QStandardItem* AddItem( int row, int col, void* parentData, const QString& text, Qt::ItemFlags flags = 0 );
    QStandardItem* AddItem( int row, int col, int rowSpan, int columnSpan, void* parentData, const QString& text, Qt::ItemFlags flags = 0 );
    QStandardItem* AddItem( int row, int col, void* parentData, ADN_Connector_ABC* data, ADN_StandardItem::E_Type type, Qt::ItemFlags flags = 0 );
    QStandardItem* AddItem( int row, int col, void* parentData, ADN_Connector_ABC* data, const QStringList& content, Qt::ItemFlags flags = 0 );
    template< typename T >
    QStandardItem* AddItem( int row, int col, void* parentData, ADN_TypePtr_InVector_ABC< T >* data, Qt::ItemFlags flags = 0 );
    ADN_TableDelegate& GetDelegate();
    const QStandardItemModel& GetModel() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void* GetData( int row, int col ) const;
    void* GetDataFromIndex( const QModelIndex& index ) const;
    void* GetSelectedData() const;
    virtual void OnContextMenu( const QPoint& pt );
    //@}

    //! @name QTableView overload
    //@{
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    virtual bool event( QEvent *event );
    virtual void mousePressEvent( QMouseEvent* mouseEvent );

    void Warn( int row, int col );
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize( const QString& objectName );
    QString GetToolTips( int nRow, int nCol ) const;
    ADN_StandardItem* AddItemInternal( int row, int col, void* parentData, ADN_Connector_ABC* data, Qt::ItemFlags flags = 0 );
    //@}

    //! @name LanguageObserver_ABC
    //@{
    virtual void OnLanguageChanged();
    //@}

    virtual void SetEnabled( bool enable );

private slots:
    //! @name Slots
    //@{
    void OnGotoRequested( const QModelIndex& index );
    void OnCheckedStateChanged( const QStandardItem& item );
    void PrivateOnContextMenu( const QPoint& pt );
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
    std::unique_ptr< QSortFilterProxyModel > proxyModel_;
    ADN_NavigationInfos::GoTo goToInfo_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Table::CreateNewElement
// Created: ABR 2013-02-12
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_Table::CreateNewElement()
{
    T* newElement = new T();
    ADN_Connector_Vector_ABC& connector = static_cast< ADN_Connector_Vector_ABC& >( GetConnector() );
    connector.AddItem( newElement );
    connector.AddItem( 0 );
    return newElement;
}

// -----------------------------------------------------------------------------
// Name: ADN_Table::AddItem
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* ADN_Table::AddItem( int row, int col, void* parentData, ADN_TypePtr_InVector_ABC< T >* data, Qt::ItemFlags flags /* = 0 */ )
{
    if( !data || !parentData )
        return 0;
    ADN_StandardItem* item = AddItemInternal( row, col, parentData, data, flags );
    if( data->GetData() )
        connect( &data->GetData()->strName_, SIGNAL( DataChanged( void* ) ), item->GetConnector(), SLOT( Rename( void* ) ) );
    return item;
}

#endif // __ADN_Table_h_
