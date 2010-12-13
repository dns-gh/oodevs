// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FireFactory_h_
#define __FireFactory_h_

#include "protocol/Protocol.h"

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
    explicit FireFactory( Model& model );
    virtual ~FireFactory();
    //@}

    //! @name Operations
    //@{
    virtual Fire_ABC* CreateFire( const sword::StartUnitFire& message );
    virtual Fire_ABC* CreateFire( const sword::StartCrowdFire& message );
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
    //@}
};

#endif // __FireFactory_h_
