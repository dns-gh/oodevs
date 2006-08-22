// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Aggregatable_ABC_h_
#define __Aggregatable_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  Aggregatable_ABC
    @brief  Aggregatable_ABC
*/
// Created: AGE 2006-04-11
// =============================================================================
class Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Aggregatable_ABC() {};
    virtual ~Aggregatable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Aggregate( const bool& ) = 0;
    //@}
};

}

#endif // __Aggregatable_ABC_h_
