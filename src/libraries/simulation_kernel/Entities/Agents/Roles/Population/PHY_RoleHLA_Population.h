// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Population_h_
#define __PHY_RoleHLA_Population_h_

#include "PHY_RoleInterface_Population.h"

// =============================================================================
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Population : public PHY_RoleInterface_Population
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Population();
    virtual ~PHY_RoleHLA_Population();
    //@}
    
    //! @name Accessors
    //@{
    virtual bool IsInvulnerable() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) {};
    virtual void Clean     () {};
    virtual bool HasChanged() const { return false; };
    virtual MT_Float ModifyMaxSpeed         ( MT_Float rSpeed    ) const { return 0; };
    virtual MT_Float GetCollidingPopulationDensity() const { return 0; };
    //@}

private:
    bool bInvulnerable_;

};

#endif // __PHY_RoleHLA_Population_h_
