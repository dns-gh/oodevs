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

#include <Qt3Support/q3listview.h>
#include <QtGui/qfont.h>
#include <QtGui/qcolor.h>
#include <Qt3Support/q3valuevector.h>
#include <boost/noncopyable.hpp>

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
class MT_ListViewItem : public Q3ListViewItem
                      , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    MT_ListViewItem( Q3ListView * parent );
    MT_ListViewItem( Q3ListView * parent, Q3ListViewItem * after );
    MT_ListViewItem( Q3ListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ListViewItem( Q3ListView * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    MT_ListViewItem( Q3ListViewItem * parent );
    MT_ListViewItem( Q3ListViewItem * parent, Q3ListViewItem * after );
    MT_ListViewItem( Q3ListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ListViewItem( Q3ListViewItem * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
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
    int width( const QFontMetrics &fm, const Q3ListView *lv, int column ) const;
    //@}

private:
    Q3ValueVector<QFont> fonts_;
    Q3ValueVector<QColor> fontColors_;
    Q3ValueVector<QColor> backgrounds_;

    QFont* pFont_;
    QColor* pFontColor_;
    QColor* pBackgroundColor_;
};

#endif // __MT_ListViewItem_h_
