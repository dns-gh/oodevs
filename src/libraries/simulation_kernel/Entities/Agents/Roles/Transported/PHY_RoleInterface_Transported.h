// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:11 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Transported.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Transported_h_
#define __PHY_RoleInterface_Transported_h_

#include "MIL.h"

#include "MT_Tools/Role_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_RoleInterface_Transported
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Transported : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Transported RoleInterface;

public:
             PHY_RoleInterface_Transported();
    virtual ~PHY_RoleInterface_Transported();

    //! @name Operations
    //@{
    virtual bool LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual bool UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable ) = 0;
    virtual bool CancelTransport    ( const MIL_Agent_ABC& transporter ) = 0;
    virtual void GetTransportWeight ( bool bTransportOnlyLoadable, MT_Float& rTotalWeight, MT_Float& rHeaviestComposanteWeight ) const = 0 ;
    virtual bool IsTransported      () const = 0;
    //@}
};


#endif // __PHY_RoleInterface_Transported_h_
