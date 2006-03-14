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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/RichListItem.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:29 $
// $Revision: 2 $
// $Workfile: RichListItem.h $
//
// *****************************************************************************

#ifndef __RichListItem_h_
#define __RichListItem_h_

// =============================================================================
/** @class  RichListItem
    @brief  Rich ListItem
*/
// Created: APE 2004-09-07
// =============================================================================
class RichListItem : public QListViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichListItem( QListView * parent );
             RichListItem( QListView * parent, QListViewItem * after );
             RichListItem( QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
             RichListItem( QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    explicit RichListItem( QListViewItem * parent );
             RichListItem( QListViewItem * parent, QListViewItem * after );
             RichListItem( QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
             RichListItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~RichListItem();
    //@}

    //! @name Operations
    //@{
    void SetBackgroundColor( const QColor& color );
    void SetBackgroundColor( const QColor& color1, const QColor& color2 );

    void SetFont( const QFont& font );
    void SetFontColor( const QColor& color );
    void SetBold( bool b );
    //@}

    //! @name Operations
    //@{
    virtual void paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign );
    virtual int  width( const QFontMetrics& fm, const QListView* pListView, int nColumn ) const;
    virtual void setText( int column, const QString& text );
    virtual QString text( int column ) const;
    //@}

private:
    //! @name Helpers
    //@{
    bool InitializeColor();
    void AddColumns( const QString& label1, const QString& label2, const QString& label3, const QString& label4, 
                     const QString& label5, const QString& label6, const QString& label7, const QString& label8 );
    const QColor& GetBackgroundColor();
    QSimpleRichText* CreateRichText( const QString& label );
    //@}

    //! @name Types
    //@{
    typedef std::pair< QString, QSimpleRichText* > T_RichText;
    typedef std::vector< T_RichText >              T_RichTexts;
    typedef T_RichTexts::iterator                 IT_RichTexts;
    //@}

private:
    //! @name Member data
    //@{
    T_RichTexts columns_;

    bool even_;
    QColor backgroundColor_;
    QColor backgroundColor2_;   // For listviews with alternating colors

    QFont font_;
    QColor fontColor_;
    //@}
};

#endif // __RichListItem_h_
