// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RoleInterface_Dotations.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:38 $
// $Revision: 2 $
// $Workfile: NET_RoleInterface_Dotations.h $
//
// *****************************************************************************

#ifndef __NET_RoleInterface_Dotations_h_
#define __NET_RoleInterface_Dotations_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  NET_RoleInterface_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class NET_RoleInterface_Dotations : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( NET_RoleInterface_Dotations )

public:
    //! @name Types
    //@{
    typedef NET_RoleInterface_Dotations RoleInterface;
    //@}

public:
    explicit NET_RoleInterface_Dotations( MT_RoleContainer& role );
             NET_RoleInterface_Dotations();
    virtual ~NET_RoleInterface_Dotations();
    
    //! @name CheckPOints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: NET_RoleInterface_Dotations::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void NET_RoleInterface_Dotations::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __NET_RoleInterface_Dotations_h_
