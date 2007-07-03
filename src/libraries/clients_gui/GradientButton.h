// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientButton_h_
#define __GradientButton_h_

#include <qcanvas.h>

namespace gui
{
    class Gradient;
    class GradientItem;

// =============================================================================
/** @class  GradientButton
    @brief  GradientButton
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientButton : public QCanvasView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit GradientButton( QWidget* parent );
    virtual ~GradientButton();
    //@}

    //! @name Operations
    //@{
    void LoadGradient( const Gradient& gradient );
    void SetSelectedColor( const QColor& color );
    GradientItem* AddItem( unsigned int percentage, const QColor& color );
    //@}

signals:
    //! @name Signals
    //@{
    void SelectionChanged( const QColor& );
    void GradientChanged( const Gradient& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GradientButton( const GradientButton& );            //!< Copy constructor
    GradientButton& operator=( const GradientButton& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );

    void SetSelected( GradientItem& item );
    void ClearSelection();
    void Update();
    //@}

    //! @name Types
    //@{
    typedef std::vector< GradientItem* > T_Colors;
    typedef T_Colors::const_iterator   CIT_Colors;
    //@}

private:
    //! @name Member data
    //@{
    T_Colors colors_;
    GradientItem* selected_;
    //@}
};

}

#endif // __GradientButton_h_