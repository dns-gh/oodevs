// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_RichListViewItem.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:29 $
// $Revision: 2 $
// $Workfile: MT_RichListViewItem.h $
//
// *****************************************************************************

#ifndef __MT_RichListViewItem_h_
#define __MT_RichListViewItem_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MT_RichListViewItem
    @brief  MT_RichListViewItem
    @par    Using example
    @code
    MT_RichListViewItem;
    @endcode
*/
// Created: APE 2004-09-07
// =============================================================================
class MT_RichListViewItem : public QListViewItem
{
    MT_COPYNOTALLOWED( MT_RichListViewItem );

public:
    //! @name Constructors/Destructor
    //@{
     MT_RichListViewItem( QListView * parent );
     MT_RichListViewItem( QListView * parent, QListViewItem * after );
     MT_RichListViewItem( QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
     MT_RichListViewItem( QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

     MT_RichListViewItem( QListViewItem * parent );
     MT_RichListViewItem( QListViewItem * parent, QListViewItem * after );
     MT_RichListViewItem( QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
     MT_RichListViewItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    ~MT_RichListViewItem();
    //@}

    //! @name Operations
    //@{
    QString GetAnchorAt( const QPoint globalPos, int nColumn ) const;

    void SetBackgroundColor( const QColor& color );
    void SetAlternatingBackgroundColors( const QColor& color1, const QColor& color2 );

    void SetFont( const QFont& font );
    void SetFontColor( const QColor& color );
    void SetBold( bool b );
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    void InitializeSizes();
    void InitializeFont();

    const QColor& GetBackgroundColor();
    //@}

    //! @name Reimplementations
    //@{
    void paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign );
    int  width( const QFontMetrics& fm, const QListView* pListView, int nColumn ) const;
    //@}

private:
    //! @name Member data
    //@{
    QValueVector<QSimpleRichText*> richText_;

    QColor currentBackgroundColor_;

    QColor backgroundColor_;
    QColor backgroundColor2_;   // For listviews with alternating colors

    QFont font_;
    QColor fontColor_;
    //@}
};

#include "MT_RichListViewItem.inl"

#endif // __MT_RichListViewItem_h_
