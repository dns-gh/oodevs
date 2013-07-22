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

namespace sword
{
    class StartUnitFire;
    class StartCrowdFire;
}

namespace kernel
{
    class Controller;
}

class Fire_ABC;
class Model;

// =============================================================================
/** @class  FireFactory
    @brief  FireFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireFactory
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FireFactory( Model& model, kernel::Controller& controller );
    virtual ~FireFactory();
    //@}

    //! @name Operations
    //@{
    virtual Fire_ABC* CreateFire( const sword::StartUnitFire& message, unsigned long id );
    virtual Fire_ABC* CreateFire( const sword::StartCrowdFire& message, unsigned long id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireFactory( const FireFactory& );            //!< Copy constructor
    FireFactory& operator=( const FireFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::Controller& controller_;
    //@}
};

#endif // __FireFactory_h_
