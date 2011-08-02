// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RichListItem_h_
#define __RichListItem_h_

#include <Qt3Support/q3listview.h>
#include <boost/noncopyable.hpp>
#include <vector>

class Q3SimpleRichText;

namespace gui
{

// =============================================================================
/** @class  RichListItem
    @brief  Rich ListItem
*/
// Created: APE 2004-09-07
// =============================================================================
class RichListItem : public Q3ListViewItem
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RichListItem( Q3ListView * parent );
             RichListItem( Q3ListView * parent, Q3ListViewItem * after );
             RichListItem( Q3ListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
             RichListItem( Q3ListView * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    explicit RichListItem( Q3ListViewItem * parent );
             RichListItem( Q3ListViewItem * parent, Q3ListViewItem * after );
             RichListItem( Q3ListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
             RichListItem( Q3ListViewItem * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~RichListItem();
    //@}

    //! @name Operations
    //@{
    void SetBackgroundColor( const QColor& color );
    void SetBackgroundColor( const QColor& color1, const QColor& color2 );
    const QColor& GetBackgroundColor();

    void SetFont( const QFont& font );
    void SetFontColor( const QColor& color );
    void SetBold( bool b );

    QString GetAnchorAt( const QPoint globalPos, int nColumn ) const;
    //@}

    //! @name Operations
    //@{
    virtual void paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign );
    virtual int  width( const QFontMetrics& fm, const Q3ListView* pListView, int nColumn ) const;
    virtual void setText( int column, const QString& text );
    virtual QString text( int column ) const;
    virtual void setPixmap( int column, const QPixmap & pm );
    virtual const QPixmap * pixmap( int column ) const;
    int rtti() const;
    //@}

private:
    //! @name Helpers
    //@{
    bool InitializeColor();
    void AddColumns( const QString& label1, const QString& label2, const QString& label3, const QString& label4,
                     const QString& label5, const QString& label6, const QString& label7, const QString& label8 );
    Q3SimpleRichText* CreateRichText( const QString& label );
    int Width( int nColumn ) const;
    int Height() const;
    //@}

    //! @name Types
    //@{
    struct RichText
    {
        RichText() : rich( 0 ), pixMap() {}
        RichText( const QString& s, Q3SimpleRichText* rich ) : base( s ), rich( rich ), pixMap() {}
        QString          base;
        Q3SimpleRichText* rich;
        QPixmap          pixMap;
    };
    typedef std::vector< RichText > T_RichTexts;
    typedef T_RichTexts::iterator  IT_RichTexts;
    //@}

private:
    //! @name Member data
    //@{
    T_RichTexts columns_;

    QColor backgroundColor_;
    QColor backgroundColor2_;   // For listviews with alternating colors
    bool even_;

    QFont font_;
    QColor fontColor_;
    //@}
};

}

#endif // __RichListItem_h_
