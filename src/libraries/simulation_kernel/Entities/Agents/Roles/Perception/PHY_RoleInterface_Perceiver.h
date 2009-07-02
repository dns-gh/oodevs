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

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleInterface_Perceiver : public MT_Role_ABC
                                  , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Perceiver RoleInterface;
    //@}

public:
    explicit PHY_RoleInterface_Perceiver( MT_RoleContainer& role );
             PHY_RoleInterface_Perceiver();
    virtual ~PHY_RoleInterface_Perceiver();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{    
    virtual bool IsUsingActiveRadar() const = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Perceiver::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleInterface_Perceiver::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Perceiver_h_
