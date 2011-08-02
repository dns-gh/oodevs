// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiniView_h_
#define __MiniView_h_

namespace kernel
{
    class Entity_ABC;
    class Positions;
    class Controllers;
    class ActionController;
}

namespace gui
{

// =============================================================================
/** @class  MiniView
    @brief  MiniView
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniView : public Q3Frame
{
public:
    //! @name Constructors/Destructor
    //@{
             MiniView( QWidget* parent, kernel::Controllers& controllers, const kernel::Entity_ABC& agent );
    virtual ~MiniView();
    //@}

    //! @name Operations
    //@{
    geometry::Rectangle2f GetViewport() const;
    void SetImage( const QImage& image );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MiniView( const MiniView& );            //!< Copy constructor
    MiniView& operator=( const MiniView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void mouseReleaseEvent( QMouseEvent * e );
    virtual void contextMenuEvent( QContextMenuEvent * e );
    virtual void mouseDoubleClickEvent( QMouseEvent * e );
    virtual void drawContents( QPainter * p );
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actions_;
    const kernel::Entity_ABC& agent_;
    const kernel::Positions& position_;
    QPixmap pixmap_;
    //@}
};

}

#endif // __MiniView_h_
