// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_ObjetDetecte.h $
// $Author: Nld $
// $Modtime: 9/03/05 15:59 $
// $Revision: 1 $
// $Workfile: MIL_RC_ObjetDetecte.h $
//
// *****************************************************************************

#ifndef __MIL_RC_ObjetDetecte_h_
#define __MIL_RC_ObjetDetecte_h_

#include "MIL.h"

#include "MIL_RC.h"

class DEC_Knowledge_Object;

// =============================================================================
// @class  MIL_RC_ObjetDetecte
// Created: JVT 2004-08-03
// =============================================================================
class MIL_RC_ObjetDetecte : public MIL_RC
{
    MT_COPYNOTALLOWED( MIL_RC_ObjetDetecte )

public:   
    MIL_RC_ObjetDetecte( E_RC nRC, int nAsnID );
    virtual ~MIL_RC_ObjetDetecte();
    
    //! @name Operations
    //@{
    virtual void Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& parameters ) const;
    virtual void Send( const MIL_Automate&  sender, E_RcType nType, DIA_Parameters& parameters ) const;
            void Send( const MIL_AgentPion& sender, E_RcType nType, const DEC_Knowledge_Object& param ) const;
    //@}
};

#endif // __MIL_RC_ObjetDetecte_h_
