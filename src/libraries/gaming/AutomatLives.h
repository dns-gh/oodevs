// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatLives_h_
#define __AutomatLives_h_

#include "Lives_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  AutomatLives
    @brief  AutomatLives
*/
// Created: AGE 2006-10-06
// =============================================================================
class AutomatLives : public Lives_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatLives( const kernel::Entity_ABC& automat, float factor );
    virtual ~AutomatLives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual float GetLife() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatLives( const AutomatLives& );            //!< Copy constructor
    AutomatLives& operator=( const AutomatLives& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& automat_;
    float factor_;
    //@}
};

#endif // __AutomatLives_h_
