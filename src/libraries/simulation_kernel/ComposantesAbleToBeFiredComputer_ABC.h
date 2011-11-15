// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ComposantesAbleToBeFiredComputer_ABC_h_
#define __ComposantesAbleToBeFiredComputer_ABC_h_

#include "simulation_kernel/OnComponentComputer_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

class PHY_ComposantePion;

namespace firing
{

// =============================================================================
/** @class  ComposantesAbleToBeFiredComputer_ABC
    @brief  ComposantesAbleToBeFiredComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class ComposantesAbleToBeFiredComputer_ABC : public OnComponentComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ComposantesAbleToBeFiredComputer_ABC() {}
    virtual ~ComposantesAbleToBeFiredComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& fired ) = 0;
    virtual PHY_Composante_ABC::T_ComposanteVector& ResultAll() = 0;
    virtual PHY_Composante_ABC::T_ComposanteVector& ResultLimited( unsigned int nNbrWeaponsUsable ) = 0;
    //@}
};

} // namespace firing

#endif // __ComposantesAbleToBeFiredComputer_ABC_h_
