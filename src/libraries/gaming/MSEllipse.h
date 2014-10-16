// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MSEllipse_h_
#define __MSEllipse_h_

namespace sword
{
    class Location;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class GLView_ABC;
}

// =============================================================================
/** @class  MSEllipse
    @brief  MSEllipse
*/
// Created: AGE 2006-04-04
// =============================================================================
class MSEllipse
{
public:
    //! @name Constructors/Destructor
    //@{
    MSEllipse( const sword::Location& localisation, const kernel::CoordinateConverter_ABC& converter );
    virtual ~MSEllipse();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const gui::GLView_ABC& tools ) const;
    virtual bool IsInside( geometry::Point2f point ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MSEllipse( const MSEllipse& );            //!< Copy constructor
    MSEllipse& operator=( const MSEllipse& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f origin_;
    float minorAxis_;
    float majorAxis_;
    float angle_;
    //@}
};

#endif // __MSEllipse_h_
