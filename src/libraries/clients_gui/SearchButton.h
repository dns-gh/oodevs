// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SearchButton_h_
#define __SearchButton_h_

#include "RichPushButton.h"

namespace gui
{

// =============================================================================
/** @class  SearchButton
    @brief  SearchButton
*/
// Created: ABR 2012-03-27
// =============================================================================
class SearchButton : public RichPushButton
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SearchButton( const QString& objectName, QWidget* parent = 0 );
    virtual ~SearchButton();
    //@}

    //! @name Operations
    //@{
    virtual void paintEvent( QPaintEvent *event );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Helpers
    //@{
    static QImage GenerateSearchImage();
    //@}

private:
    //! @name Member data
    //@{
    QImage image_;
    //@}
};

} //! namespace gui

#endif // __SearchButton_h_
