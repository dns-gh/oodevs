// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceTransportConsign.h $
// $Author: Jvt $
// $Modtime: 11/04/05 16:56 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceTransportConsign_h_
#define __PHY_MaintenanceTransportConsign_h_

#include "MIL.h"

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_RolePionLOG_Maintenance;

// =============================================================================
// @class  PHY_MaintenanceTransportConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceTransportConsign : public PHY_MaintenanceConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_MaintenanceTransportConsign )

public:
             PHY_MaintenanceTransportConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState );
             PHY_MaintenanceTransportConsign();
    virtual ~PHY_MaintenanceTransportConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual bool Update(); 
    //@}
};

#include "PHY_MaintenanceTransportConsign.inl"

#endif // __PHY_MaintenanceTransportConsign_h_
