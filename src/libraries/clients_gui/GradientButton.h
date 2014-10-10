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

#include "GradientItem.h"

namespace gui
{
    class Gradient;

// =============================================================================
/** @class  GradientButton
    @brief  GradientButton
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientButton : public Q3CanvasView
{
    Q_OBJECT;
public:
    //! @name Types
    //@{
    typedef std::shared_ptr< Gradient > T_Gradient;
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             GradientButton( const QString& objectName,
                             bool disableState,
                             const GradientItem::T_Drawer& itemDrawer,
                             QColor begin = Qt::green,
                             QColor end = Qt::red,
                             QWidget* parent = 0 );
    virtual ~GradientButton();
    //@}

    //! @name Operations
    //@{
    void LoadGradient( const T_Gradient& gradient );
    void SetSelectedColor( const QColor& color );
    QColor GetSelectedColor() const;
    GradientItem* AddItem( unsigned int percentage, const QColor& color );
    //@}

signals:
    //! @name Signals
    //@{
    void SelectionChanged( const QColor& );
    void GradientChanged();
    //@}

private slots:
    //! @name slots
    //@{
    void OnFitToViewPortChanged( int state );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void resizeEvent( QResizeEvent* event );

    void SetSelected( GradientItem& item );
    void ClearSelection();
    void Update( bool emitSignal = true );
    //@}

private:
    //! @name Member data
    //@{
    GradientItem::T_Drawer itemDrawer_;
    T_Gradient gradient_;
    std::vector< GradientItem* > colors_;
    GradientItem* selected_;
    bool disableState_;
    //@}
};

} //! namespace gui

#endif // __GradientButton_h_