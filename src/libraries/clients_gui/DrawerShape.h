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
    explicit DrawerShape( const DrawerStyle& style );
    virtual ~DrawerShape();
    //@}

    //! @name Construction
    //@{
    void PopPoint();
    void AddPoint( const geometry::Point2f& point );
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerShape( const DrawerShape& );            //!< Copy constructor
    DrawerShape& operator=( const DrawerShape& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const DrawerStyle& style_;
    T_PointVector points_;
    //@}
};

}

#endif // __DrawerShape_h_
