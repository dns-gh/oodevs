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
             MIL_AgentTypePionLOGTC2( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOGTC2();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const;
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const; 
    //@}

    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#endif // __MIL_AgentTypePionLOGTC2_h_
