// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Diplomacies_ABC_h_
#define __Diplomacies_ABC_h_

#include "Extension_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Karma;

// =============================================================================
/** @class  Diplomacies_ABC
    @brief  Diplomacies_ABC
*/
// Created: SBO 2008-12-09
// =============================================================================
class Diplomacies_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies_ABC() {}
    virtual ~Diplomacies_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Karma& GetDiplomacy( const kernel::Entity_ABC& entity ) const = 0;
    virtual const kernel::Karma& GetKarma() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Diplomacies_ABC( const Diplomacies_ABC& );            //!< Copy constructor
    Diplomacies_ABC& operator=( const Diplomacies_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __Diplomacies_ABC_h_
