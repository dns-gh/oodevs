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

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

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
class AutomatLives : public kernel::Extension_ABC
                   , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatLives( const kernel::Entity_ABC& automat );
    virtual ~AutomatLives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatLives( const AutomatLives& );            //!< Copy constructor
    AutomatLives& operator=( const AutomatLives& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    float GetLife() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& automat_;
    //@}
};

#endif // __AutomatLives_h_
