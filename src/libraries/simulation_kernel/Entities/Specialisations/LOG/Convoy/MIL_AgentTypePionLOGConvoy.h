// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Convoy/MIL_AgentTypePionLOGConvoy.h $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGConvoy.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOGConvoy_h_
#define __MIL_AgentTypePionLOGConvoy_h_

#include "Entities/Specialisations/LOG/MIL_AgentTypePionLOG_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOGConvoy : public MIL_AgentTypePionLOG_ABC
{
public:
             MIL_AgentTypePionLOGConvoy( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOGConvoy();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion* InstanciatePion( MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const;
    virtual void RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const;
    //@}

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );

protected:
    virtual void DeleteUnit( MIL_Agent_ABC& unit ) const;
};

#endif // __MIL_AgentTypePionLOGConvoy_h_
