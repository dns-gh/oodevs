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
             VerticalHeaderStyle( QStyle *style );
    virtual ~VerticalHeaderStyle();
    //@}

    //! @name Operations
    //@{
    virtual void drawControl( ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = 0 ) const;
    //@}
};

// =============================================================================
/** @class  VerticalHeaderTableView
    @brief  VerticalHeaderTableView
*/
// Created: ABR 2012-02-24
// =============================================================================
class VerticalHeaderTableView : public QTableView
{
public:
    //! @name Constructors/Destructor
    //@{
             VerticalHeaderTableView( QWidget* parent );
    virtual ~VerticalHeaderTableView();
    //@}
};

}

#endif // __VerticalHeaderTableView_h_
