// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireFactory_h_
#define __FireFactory_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class StartUnitFire;
    class StartCrowdFire;
    class StartUnitFireDetection;
}

namespace kernel
{
    class Controller;
    class Profile_ABC;
}

class Fire_ABC;
class Model;

// =============================================================================
/** @class  FireFactory
    @brief  FireFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireFactory : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FireFactory( Model& model, kernel::Controller& controller, const kernel::Profile_ABC& profile );
    virtual ~FireFactory();
    //@}

    //! @name Operations
    //@{
    Fire_ABC* CreateFire( const sword::StartUnitFire& message, unsigned long id );
    Fire_ABC* CreateFire( const sword::StartCrowdFire& message, unsigned long id );
    Fire_ABC* CreateFire( const sword::StartUnitFireDetection& message, unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::Controller& controller_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __FireFactory_h_
