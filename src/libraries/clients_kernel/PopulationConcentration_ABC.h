// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_ABC_h_
#define __PopulationConcentration_ABC_h_

#include "PopulationPart_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  PopulationConcentration_ABC
    @brief  PopulationConcentration_ABC
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationConcentration_ABC : public PopulationPart_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration_ABC() {}
    virtual ~PopulationConcentration_ABC() {}
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration_ABC( const PopulationConcentration_ABC& );
    PopulationConcentration_ABC& operator=( const PopulationConcentration_ABC& );
    //@}
};

}

#endif // __PopulationConcentration_ABC_h_
