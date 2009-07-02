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

#include "MIL.h"

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
             MIL_AgentTypePionLOGConvoy( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionLOGConvoy();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const;
    virtual MIL_AgentPion& InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const; 
    //@}

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#include "MIL_AgentTypePionLOGConvoy.inl"

#endif // __MIL_AgentTypePionLOGConvoy_h_
