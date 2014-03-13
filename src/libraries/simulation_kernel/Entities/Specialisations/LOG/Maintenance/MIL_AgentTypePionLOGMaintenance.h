// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Maintenance/MIL_AgentTypePionLOGMaintenance.h $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMaintenance.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOGMaintenance_h_
#define __MIL_AgentTypePionLOGMaintenance_h_

#include "Entities/Specialisations/LOG/MIL_AgentTypePionLOG_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOGMaintenance : public MIL_AgentTypePionLOG_ABC
{
public:
             MIL_AgentTypePionLOGMaintenance( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOGMaintenance();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion* InstanciatePion( MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const;
    virtual void RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const;
    //@}
    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );
};

#endif // __MIL_AgentTypePionLOGMaintenance_h_
