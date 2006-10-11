#ifndef	__MIL_RC_UniteTropDistante_h_
#define __MIL_RC_UniteTropDistante_h_

#include "MIL.h"

#include "MIL_RC.h"

// =============================================================================
// @class  MIL_RC_UniteTropDistante
// Created: JVT 2006-10-06
// =============================================================================
class MIL_RC_UniteTropDistante : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_UniteTropDistante )

public:   
    MIL_RC_UniteTropDistante( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_UniteTropDistante();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&   sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Population& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    //@}
};

#endif // __MIL_RC_UniteTropDistante_h_
