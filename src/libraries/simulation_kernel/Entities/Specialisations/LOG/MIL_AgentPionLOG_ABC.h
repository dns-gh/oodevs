// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:32 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOG_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOG_ABC_h_
#define __MIL_AgentPionLOG_ABC_h_

#include "MIL.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "MIL_AutomateLOG.h"

namespace xml
{
    class xistream;
}

template < typename T > class PHY_ActionLogistic;

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOG_ABC : public MIL_AgentPion
{
    MT_COPYNOTALLOWED( MIL_AgentPionLOG_ABC )

public:
    MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOG_ABC();
    virtual ~MIL_AgentPionLOG_ABC();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    virtual const MIL_AutomateLOG& GetAutomate() const;
    virtual       MIL_AutomateLOG& GetAutomate();
    //@}

    //! @name Operations
    //@{
    void UpdateLogistic(); // Called by the 'PHY_ActionLogistic' action 
    //@}

private:
    PHY_ActionLogistic< MIL_AgentPionLOG_ABC >* pLogisticAction_;
};

#include "MIL_AgentPionLOG_ABC.inl"

#endif // __MIL_AgentPionLOG_ABC_h_
