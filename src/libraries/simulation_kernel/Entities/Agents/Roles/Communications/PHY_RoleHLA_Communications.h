// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RoleHLA_Communications.h $
// $Author: Age $
// $Modtime: 2/12/04 11:46 $
// $Revision: 4 $
// $Workfile: PHY_RoleHLA_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Communications_h_
#define __PHY_RoleHLA_Communications_h_

#include "PHY_RoleInterface_Communications.h"

// =============================================================================
/** @class  PHY_RoleHLA_Communications
    @brief  HLA role communications
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Communications : public PHY_RoleInterface_Communications
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Communications( MT_RoleContainer& roleContainer );
    virtual ~PHY_RoleHLA_Communications();
    //@}

    //! @name Operations
    //@{
    virtual bool CanCommunicate() const;
    virtual void Jam           ( const MIL_Object_ABC& jammer );
    virtual void Unjam         ( const MIL_Object_ABC& jammer );
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Member data
    //@{
    bool bJammed_;
    //@}
};

#endif // __PHY_RoleHLA_Communications_h_
