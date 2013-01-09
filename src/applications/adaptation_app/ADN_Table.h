// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Table_h_
#define __ADN_Table_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_NavigationInfos.h"

//*****************************************************************************
// Created: JDY 03-07-07
//*****************************************************************************
class ADN_Table : public Q3Table, public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_Table( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Table();

    virtual void FinalizeConnection();
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