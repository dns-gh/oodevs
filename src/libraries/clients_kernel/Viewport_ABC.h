// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Viewport_ABC_h_
#define __Viewport_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  Viewport_ABC
    @brief  Viewport_ABC
*/
// Created: AGE 2007-02-23
// =============================================================================
class Viewport_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Viewport_ABC();
    virtual ~Viewport_ABC();
    //@}

    //! @name Operations
    //@{
    bool IsHotpointVisible() const;
    virtual bool IsVisible( const geometry::Point2f& point ) const = 0;
    virtual bool IsVisible( const geometry::Rectangle2f& rectangle ) const = 0;

    void SetHotpoint( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Viewport_ABC( const Viewport_ABC& );            //!< Copy constructor
    Viewport_ABC& operator=( const Viewport_ABC& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    bool hotIsVisible_;
    //@}
};

}

#endif // __Viewport_ABC_h_
