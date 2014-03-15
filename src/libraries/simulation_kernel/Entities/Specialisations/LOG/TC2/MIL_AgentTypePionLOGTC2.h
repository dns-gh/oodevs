// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Tc2/MIL_AgentTypePionLOGTC2.h $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGTC2.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionLOGTC2_h_
#define __MIL_AgentTypePionLOGTC2_h_

#include "Entities/Specialisations/LOG/MIL_AgentTypePionLOG_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionLOGTC2 : public MIL_AgentTypePionLOG_ABC
{
public:
             MIL_AgentTypePionLOGTC2( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOGTC2();

    //! @name Instanciation
    //@{
    virtual void RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const;
    //@}

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );

protected:
    void DeleteUnit( MIL_Agent_ABC& unit ) const;
};

#endif // __MIL_AgentTypePionLOGTC2_h_
