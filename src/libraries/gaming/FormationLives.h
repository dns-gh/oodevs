// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationLives_h_
#define __FormationLives_h_

#include "Lives_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  FormationLives
    @brief  FormationLives
*/
// Created: LGY 2011-03-09
// =============================================================================
class FormationLives : public Lives_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FormationLives( const kernel::Entity_ABC& formation );
    virtual ~FormationLives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual float GetLife() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationLives( const FormationLives& );            //!< Copy constructor
    FormationLives& operator=( const FormationLives& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& formation_;
    //@}
};

#endif // __FormationLives_h_
