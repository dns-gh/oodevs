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

class Entity_ABC;
class Positions;
class GlWidget;
class Controllers;
class ActionController;

// =============================================================================
/** @class  MiniView
    @brief  MiniView
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniView : public QFrame
{

public:
    //! @name Constructors/Destructor
    //@{
             MiniView( QWidget* parent, Controllers& controllers, const Entity_ABC& agent, GlWidget* widget );
    virtual ~MiniView();
    //@}

    //! @name Operations
    //@{
    geometry::Rectangle2f GetViewport() const;
    void SetImage( const QImage& image );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MiniView( const MiniView& );            //!< Copy constructor
    MiniView& operator=( const MiniView& ); //!< Assignement operator
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
    ActionController& actions_;
    const Entity_ABC& agent_;
    const Positions& position_;
    QPixmap pixmap_;
    GlWidget* widget_;

    //@}
};

#endif // __MiniView_h_
