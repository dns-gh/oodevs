// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:05 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_HumanFactors.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_HumanFactors_h_
#define __PHY_RoleInterface_HumanFactors_h_

#include "MT_Tools/MT_Role_ABC.h"

class MIL_NbcAgentType;

// =============================================================================
// @class  PHY_RoleInterface_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_HumanFactors : public MT_Role_ABC
                                     , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_HumanFactors RoleInterface;

public:
    explicit PHY_RoleInterface_HumanFactors( MT_RoleContainer& role );
             PHY_RoleInterface_HumanFactors();
    virtual ~PHY_RoleInterface_HumanFactors();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_HumanFactors::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_HumanFactors::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_HumanFactors_h_
