// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultComposantesAbleToBeFiredComputer_h_
#define __DefaultComposantesAbleToBeFiredComputer_h_

#include "simulation_kernel/ComposantesAbleToBeFiredComputer_ABC.h"

class PHY_DirectFireData;

namespace firing
{

// =============================================================================
/** @class  DefaultComposantesAbleToBeFiredComputer
    @brief  DefaultComposantesAbleToBeFiredComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultComposantesAbleToBeFiredComputer : public ComposantesAbleToBeFiredComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultComposantesAbleToBeFiredComputer( bool bFireOnlyOnMajorComposantes );
    virtual ~DefaultComposantesAbleToBeFiredComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComposante( PHY_ComposantePion& fired );
    virtual PHY_RoleInterface_Composantes::T_ComposanteVector& ResultAll();
    virtual PHY_RoleInterface_Composantes::T_ComposanteVector& ResultLimited( unsigned int nNbrWeaponsUsable );
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleInterface_Composantes::T_ComposanteVector availableTargets_;
    PHY_RoleInterface_Composantes::T_ComposanteVector targets_;
    bool bFireOnlyOnMajorComposantes_;
    //@}
};

} // namespace firing

#endif // __DefaultComposantesAbleToBeFiredComputer_h_
