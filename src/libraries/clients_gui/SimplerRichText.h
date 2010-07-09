// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SimplerRichText_h_
#define __SimplerRichText_h_

namespace gui
{

// =============================================================================
/** @class  SimplerRichText
    @brief  SimplerRichText
*/
// Created: AGE 2007-04-20
// =============================================================================
class SimplerRichText
{
public:
    //! @name Constructors/Destructor
    //@{
             SimplerRichText( const QString& text, const QFont& font );
    virtual ~SimplerRichText();
    //@}

    //! @name Operations
    //@{
    int height() const;
    void setDefaultFont( const QFont& font );
    void draw( QPainter* p, int x, int y, const QRect& clipRect, const QColorGroup& cg, const QBrush* paper = 0 ) const;
    QString anchorAt( const QPoint& point ) const;
    void setText( const QString& text );
    int widthUsed() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimplerRichText( const SimplerRichText& );            //!< Copy constructor
    SimplerRichText& operator=( const SimplerRichText& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    class RichElement;
    typedef std::vector< RichElement >   T_Elements;
    typedef T_Elements::iterator        IT_Elements;
    typedef T_Elements::const_iterator CIT_Elements;

    class RichElement
    {
    public:
        RichElement( SimplerRichText* parent, const QString& text, const QString& link );
        void draw( QPainter* p, int x, int y, const QColorGroup& cg ) const;
        int width() const;
        int widthUsed() const;
        bool IsAt( const QPoint& point ) const;
        const QString& Anchor() const;
        void UpdateOffset( SimplerRichText::CIT_Elements it );

    private:
        SimplerRichText* parent_;
        QString label_;
        QString anchor_;
        int offset_;
    };
    //@}

    //! @name Helpers
    //@{
    void Parse( const QString& text );
    void Breakdown( const QString& text, QStringList& list );
    void AddTextElement( const QString& text );
    void AddLinkElement( const QString& text );
    void AddLinkElement( const QString& link, const QString& text );
    void UpdateOffset();
    //@}

private:
    //! @name Member data
    //@{
    QFont font_;
    QFont linkFont_;
    T_Elements elements_;
    //@}
};

}

#endif // __SimplerRichText_h_
