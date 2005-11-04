// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleHLA_Population.h $
// $Author: Age $
// $Modtime: 2/12/04 11:45 $
// $Revision: 3 $
// $Workfile: PHY_RoleHLA_Population.h $
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
             PHY_RoleHLA_Population( MT_RoleContainer& role );
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

private:
    bool bInvulnerable_;

};

#endif // __PHY_RoleHLA_Population_h_
