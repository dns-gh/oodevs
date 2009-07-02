// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RoleInterface_Maintenance.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:17 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Maintenance_h_
#define __PHY_RoleInterface_Maintenance_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Maintenance : public MT_Role_ABC
                                    , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Maintenance RoleInterface;
    //@}

public:
    explicit PHY_RoleInterface_Maintenance( MT_RoleContainer& role );
             PHY_RoleInterface_Maintenance();
    virtual ~PHY_RoleInterface_Maintenance();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Maintenance::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Maintenance::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Maintenance_h_
