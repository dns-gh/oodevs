// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_PopulationVictimeAffrontements.h $
// $Author: Nld $
// $Modtime: 9/03/05 18:33 $
// $Revision: 1 $
// $Workfile: MIL_RC_PopulationVictimeAffrontements.h $
//
// *****************************************************************************

#ifndef __MIL_RC_PopulationVictimeAffrontements_h_
#define __MIL_RC_PopulationVictimeAffrontements_h_

#include "MIL.h"

#include "MIL_RC.h"

// =============================================================================
// @class  MIL_RC_PopulationVictimeAffrontements
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_PopulationVictimeAffrontements : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_PopulationVictimeAffrontements )

public:   
    MIL_RC_PopulationVictimeAffrontements( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_PopulationVictimeAffrontements();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&   sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Population& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    
            void Send( const MIL_AgentPion&  sender, E_RcType nType, uint nNNbrVictim, uint nNNbrWounded ) const;
            void Send( const MIL_Population& sender, E_RcType nType, uint nNNbrVictim, uint nNNbrWounded ) const;
    //@}
};

#endif // __MIL_RC_PopulationVictimeAffrontements_h_
