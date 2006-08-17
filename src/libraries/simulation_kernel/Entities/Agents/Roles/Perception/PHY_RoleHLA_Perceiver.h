// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleHLA_Perceiver.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Perceiver_h_
#define __PHY_RoleHLA_Perceiver_h_

#include "MIL.h"

#include "PHY_RoleInterface_Perceiver.h"

// =============================================================================
// @class  PHY_RoleHLA_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleHLA_Perceiver : public PHY_RoleInterface_Perceiver
{
    MT_COPYNOTALLOWED( PHY_RoleHLA_Perceiver )

public:
    PHY_RoleHLA_Perceiver( MT_RoleContainer& role );
    virtual ~PHY_RoleHLA_Perceiver();

    //! @name Operations
    //@{    
    virtual bool IsUsingActiveRadar() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    bool bIsUsingActiveRadar_; 
};

#include "PHY_RoleHLA_Perceiver.inl"

#endif // __PHY_RoleHLA_Perceiver_h_
