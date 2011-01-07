// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientItem_h_
#define __GradientItem_h_

#include <qcanvas.h>

namespace gui
{
    class Painter_ABC;
// =============================================================================
/** @class  GradientItem
    @brief  GradientItem
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientItem : public QCanvasLine
{
public:
    //! @name Constructors/Destructor
    //@{
             GradientItem( QCanvas* canvas, const Painter_ABC& painter,
                           unsigned short percentage, const QColor& color, bool disableState );
    virtual ~GradientItem();
    //@}

    //! @name Operations
    //@{
    void ToggleScale( bool state );
    void SetColor( const QColor& color );
    QColor GetColor() const;
    unsigned short GetPercentage() const;
    void SetPercentage( unsigned short percentage );
    virtual void draw( QPainter& painter );
    virtual QPointArray areaPoints() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GradientItem( const GradientItem& );            //!< Copy constructor
    GradientItem& operator=( const GradientItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdatePosition();
    unsigned short GetX();
    //@}

private:
    //! @name Member data
    //@{
    unsigned short percentage_;
    const Painter_ABC& painter_;
    QColor color_;
    bool disableState_;
    //@}
};

}

#endif // __GradientItem_h_
