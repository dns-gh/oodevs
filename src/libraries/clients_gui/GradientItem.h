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

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  GradientItem
    @brief  GradientItem
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientItem : public Q3CanvasLine
                   , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::function< void ( QPainter&, unsigned int, int, int ) > T_Drawer;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GradientItem( Q3Canvas* canvas,
                           const T_Drawer& drawFunctor,
                           unsigned short percentage,
                           const QColor& color,
                           bool disableState );
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
    virtual Q3PointArray areaPoints() const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdatePosition();
    unsigned short GetX();
    //@}

private:
    //! @name Member data
    //@{
    const T_Drawer drawer_;
    unsigned short percentage_;
    QColor color_;
    bool disableState_;
    //@}
};

} //! namespace gui

#endif // __GradientItem_h_
