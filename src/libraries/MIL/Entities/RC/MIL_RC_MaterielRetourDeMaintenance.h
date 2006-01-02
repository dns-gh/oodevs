// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_MaterielRetourDeMaintenance.h $
// $Author: Nld $
// $Modtime: 9/03/05 16:06 $
// $Revision: 1 $
// $Workfile: MIL_RC_MaterielRetourDeMaintenance.h $
//
// *****************************************************************************

#ifndef __MIL_RC_MaterielRetourDeMaintenance_h_
#define __MIL_RC_MaterielRetourDeMaintenance_h_

#include "MIL.h"

#include "MIL_RC.h"

class PHY_ComposanteType_ABC;

// =============================================================================
// @class  MIL_RC_MaterielRetourDeMaintenance
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_MaterielRetourDeMaintenance : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_MaterielRetourDeMaintenance )

public:   
    MIL_RC_MaterielRetourDeMaintenance( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_MaterielRetourDeMaintenance();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
            void Send( const MIL_AgentPion& sender, E_RcType nType, const PHY_ComposanteType_ABC& type ) const;
    //@}
};

#endif // __MIL_RC_MaterielRetourDeMaintenance_h_
