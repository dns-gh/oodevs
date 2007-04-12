// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Savable_ABC_h_
#define __Savable_ABC_h_

class OutputBinaryWrapper;

namespace dispatcher
{

// =============================================================================
/** @class  Savable_ABC
    @brief  Savable_ABC
*/
// Created: AGE 2007-04-10
// =============================================================================
class Savable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Savable_ABC() {};
    virtual ~Savable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( OutputBinaryWrapper& output ) const = 0;
    //@}
};

}

#endif // __Savable_ABC_h_
