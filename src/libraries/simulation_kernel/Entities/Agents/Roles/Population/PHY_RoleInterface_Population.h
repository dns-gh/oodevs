// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleInterface_Population.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:31 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Population.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Population_h_
#define __PHY_RoleInterface_Population_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Population : public MT_Role_ABC
                                   , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Population RoleInterface;

public:
    explicit PHY_RoleInterface_Population( MT_RoleContainer& role );
             PHY_RoleInterface_Population();
    virtual ~PHY_RoleInterface_Population();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInvulnerable() const = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Population::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleInterface_Population::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Population_h_
