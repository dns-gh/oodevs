// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Knowledge_ABC_h_
#define __Knowledge_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Knowledge_ABC
    @brief  Knowledge_ABC
*/
// Created: SBO 2006-10-12
// =============================================================================
class Knowledge_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Knowledge_ABC() {}
    virtual ~Knowledge_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const Entity_ABC* GetEntity() const = 0;
    virtual const Entity_ABC* GetRecognizedEntity() const = 0;
    virtual const Entity_ABC& GetOwner() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Knowledge_ABC( const Knowledge_ABC& );
    Knowledge_ABC& operator=( const Knowledge_ABC& );
    //@}
};

}

#endif // __Knowledge_ABC_h_
