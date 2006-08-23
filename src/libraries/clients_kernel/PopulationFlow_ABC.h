// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_ABC_h_
#define __PopulationFlow_ABC_h_

#include "PopulationPart_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  PopulationFlow_ABC
    @brief  PopulationFlow_ABC
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationFlow_ABC : public PopulationPart_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow_ABC() {}
    virtual ~PopulationFlow_ABC() {}
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow_ABC( const PopulationFlow_ABC& );
    PopulationFlow_ABC& operator=( const PopulationFlow_ABC& );
    //@}
};

}

#endif // __PopulationFlow_ABC_h_
