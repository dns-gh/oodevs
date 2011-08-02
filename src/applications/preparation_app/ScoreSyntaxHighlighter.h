// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreSyntaxHighlighter_h_
#define __ScoreSyntaxHighlighter_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace indicators
{
    class Primitives;
}

// =============================================================================
/** @class  ScoreSyntaxHighlighter
    @brief  ScoreSyntaxHighlighter
*/
// Created: SBO 2009-05-07
// =============================================================================
class ScoreSyntaxHighlighter : public Q3SyntaxHighlighter
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                             , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoreSyntaxHighlighter( Q3TextEdit* editor, kernel::Controllers& controllers, const indicators::Primitives& primitives );
    virtual ~ScoreSyntaxHighlighter();
    //@}

    //! @name Operations
    //@{
    virtual int highlightParagraph( const QString& text, int endStateOfLastPara );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreSyntaxHighlighter( const ScoreSyntaxHighlighter& );            //!< Copy constructor
    ScoreSyntaxHighlighter& operator=( const ScoreSyntaxHighlighter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Highlight( const QString& regexp, const QString& text, const QFont& font, const QColor& color );
    void Highlight( const QStringList& elements, const QString& text, const QFont& font, const QColor& color );

    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const indicators::Primitives& primitives_;
    QStringList extractors_, functions_;
    QFont varFont_, extractorFont_, functionFont_;
    const QColor varColor_, extractorColor_, functionColor_;
    //@}
};

#endif // __ScoreSyntaxHighlighter_h_
