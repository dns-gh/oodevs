// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Perceiver_h_
#define __PHY_RoleInterface_Perceiver_h_

#include "MT_Tools/Role_ABC.h"

//namespace simulation
//{
//namespace perception
//{

// =============================================================================
// @class  PHY_RoleInterface_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleInterface_Perceiver : public tools::Role_ABC
                                  , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Perceiver RoleInterface;
    //@}

public:
             PHY_RoleInterface_Perceiver();
    virtual ~PHY_RoleInterface_Perceiver();

    //! @name Operations
    //@{    
    virtual bool IsUsingActiveRadar() const = 0;
    //@}
};

//} // namespace perception
//} // namespace simulation
#endif // __PHY_RoleInterface_Perceiver_h_
