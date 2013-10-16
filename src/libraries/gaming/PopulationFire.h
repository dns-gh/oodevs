// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFire_h_
#define __PopulationFiree_h_

#include "Fire_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Population_ABC;
}

namespace sword
{
    class StartCrowdFire;
}

// =============================================================================
/** @class  PopulationFire
    @brief  PopulationFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class PopulationFire : public Fire_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFire( const sword::StartCrowdFire& message, const tools::Resolver_ABC< kernel::Population_ABC >& resolver );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    //@}
};

#endif // __PopulationFire_h_
