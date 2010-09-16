// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_ListViewItem_h_
#define __MT_ListViewItem_h_

#include <qlistview.h>
#include <qfont.h>
#include <qcolor.h>
#include <qvaluevector.h> 

// =============================================================================
/** @class  MT_ListViewItem
    @brief  MT_ListViewItem
    @par    Using example
    @code
    MT_ListViewItem;
    @endcode
*/
// Created: APE 2004-03-16
// =============================================================================
class MT_ListViewItem : public QListViewItem
{
    MT_COPYNOTALLOWED( MT_ListViewItem );

public:
    //! @name Constructors/Destructor
    //@{
    MT_ListViewItem( QListView * parent );
    MT_ListViewItem( QListView * parent, QListViewItem * after );
    MT_ListViewItem( QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ListViewItem( QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    MT_ListViewItem( QListViewItem * parent );
    MT_ListViewItem( QListViewItem * parent, QListViewItem * after );
    MT_ListViewItem( QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ListViewItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~MT_ListViewItem();
    //@}

    //! @name Operations
    //@{
    void SetFont( uint nColumn, const QFont &font );
    void SetFontColor( uint nColumn, const QColor &color );
    void SetBackground( uint nColumn, const QColor &color );

    void SetFont( const QFont &font );
    void SetFontColor( const QColor &color );
    void SetBackground( const QColor &color );
    //@}

protected:
    //! @name Redefinitions
    //@{
    void paintCell( QPainter *painter, const QColorGroup &cg, int column, int width, int align );
    int width( const QFontMetrics &fm, const QListView *lv, int column ) const;
    //@}

private:
    QValueVector<QFont> fonts_;
    QValueVector<QColor> fontColors_;
    QValueVector<QColor> backgrounds_;

    QFont* pFont_;
    QColor* pFontColor_;
    QColor* pBackgroundColor_;
};

#endif // __MT_ListViewItem_h_
