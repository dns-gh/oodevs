// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RichLabel_h_
#define __RichLabel_h_

#include "RichWarnWidget.h"

namespace gui
{
    class SimplerRichText;

// =============================================================================
/** @class  RichLabel
    @brief  Rich label
*/
// Created: SBO 2006-04-18
// =============================================================================
class RichLabel : public RichWarnWidget< QLabel >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichLabel( const QString& objectName, QWidget* parent = 0 );
    explicit RichLabel( const QString& objectName, const QString& text, QWidget* parent = 0 );
             RichLabel( const QString& objectName, const QString& text, bool required, QWidget* parent = 0 );
    virtual ~RichLabel();
    //@}

    //! @name Operations
    //@{
    virtual void setText( const QString& );
    //@}

signals:
    //! @name Signals
    //@{
    void LinkClicked( const QString& anchor );
    //@}

protected:
    //! @name Events
    //@{
    virtual void mouseMoveEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent* e );
    virtual void drawContents( QPainter* p );
    //@}

private:
    //! @name Member data
    //@{
    SimplerRichText* richText_;
    //@}
};

}

#endif // __RichLabel_h_
