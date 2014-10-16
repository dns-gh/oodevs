// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConvexHulls_h_
#define __ConvexHulls_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  ConvexHulls
    @brief  ConvexHulls
*/
// Created: AGE 2007-05-30
// =============================================================================
class ConvexHulls : public kernel::Extension_ABC
                  , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ConvexHulls( const kernel::Entity_ABC& holder );
    virtual ~ConvexHulls();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

protected:
    //! @name Operations
    //@{
    void SetDead( bool dead );
    void SetPosition( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConvexHulls( const ConvexHulls& );            //!< Copy constructor
    ConvexHulls& operator=( const ConvexHulls& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsDead() const;
    void Update() const;
    void Update        ( T_PointVector& points, geometry::Point2f& leftMost, geometry::Point2f& rightMost ) const;
    void ComputeHull   ( CIT_PointVector from, CIT_PointVector to ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& holder_;
    mutable bool dead_;
    mutable T_PointVector hull_;
    mutable geometry::Point2f position_, leftMost_, rightMost_;
    //@}
};

#endif // __ConvexHulls_h_
