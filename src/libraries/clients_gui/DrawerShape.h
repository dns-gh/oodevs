// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerShape_h_
#define __DrawerShape_h_

namespace gui
{
    class DrawerStyle;

// =============================================================================
/** @class  DrawerShape
    @brief  DrawerShape
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerShape
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShape( const DrawerStyle& style, const QColor& color );
    virtual ~DrawerShape();
    //@}

    //! @name Construction
    //@{
    void PopPoint();
    void AddPoint( const geometry::Point2f& point );

    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    //@}

    //! @name Operations
    //@{
    bool IsAt( const geometry::Point2f& point, float precision = 100.f ) const;

    void Draw( const geometry::Rectangle2f& viewport, bool overlined ) const;
    void Draw( const geometry::Rectangle2f& viewport, const QColor& color, bool overlined ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerShape( const DrawerShape& );            //!< Copy constructor
    DrawerShape& operator=( const DrawerShape& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    
    //@}

private:
    //! @name Member data
    //@{
    const DrawerStyle& style_;
    QColor color_;
    QColor complement_;
    T_PointVector points_;
    //@}
};

}

#endif // __DrawerShape_h_
