// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleInterface_NBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:31 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_NBC.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_NBC_h_
#define __PHY_RoleInterface_NBC_h_

#include "MT_Tools/MT_Role_ABC.h"

class MIL_ToxicEffectManipulator;

// =============================================================================
// @class  PHY_RoleInterface_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_NBC : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_NBC )

public:
    typedef PHY_RoleInterface_NBC RoleInterface;

public:
    explicit PHY_RoleInterface_NBC( MT_RoleContainer& role );
             PHY_RoleInterface_NBC();
    virtual ~PHY_RoleInterface_NBC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual bool IsContaminated() const = 0;
    virtual void Poison        ( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Contaminate   ( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Decontaminate () = 0;
    virtual void Decontaminate ( MT_Float rRatioAgentsWorking ) = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_NBC::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleInterface_NBC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_NBC_h_
