// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __VerticalHeaderTableView_h_
#define __VerticalHeaderTableView_h_

#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  VerticalHeaderStyle
    @brief  VerticalHeaderStyle
*/
// Created: ABR 2012-02-24
// =============================================================================
class VerticalHeaderStyle : public QProxyStyle
{
public:
    //! @name Constructors/Destructor
    //@{
             VerticalHeaderStyle();
    virtual ~VerticalHeaderStyle();
    //@}

    //! @name Operations
    //@{
    virtual void drawControl( ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< QStyle > baseStyle_;
    //@}
};

// =============================================================================
/** @class  VerticalHeaderTableView
    @brief  VerticalHeaderTableView
*/
// Created: ABR 2012-02-24
// =============================================================================
class VerticalHeaderTableView : public RichWidget< QTableView >
{
public:
    //! @name Constructors/Destructor
    //@{
             VerticalHeaderTableView( QWidget* parent );
    virtual ~VerticalHeaderTableView();
    //@}

    //! @name Operations
    //@{
    virtual bool event( QEvent* event );
    //@}
};

}

#endif // __VerticalHeaderTableView_h_
