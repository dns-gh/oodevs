// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypeInjuredHumanPion.h $
// $Author: Rft $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypeInjuredHumanPion.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypeInjuredHumanPion_h_
#define __MIL_AgentTypeInjuredHumanPion_h_

#include "MIL.h"
#include "Entities/Agents/MIL_AgentTypePion.h"

namespace xml
{
    class xistream;
}

class AlgorithmsFactories;

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: RFT 2004-08-03
// =============================================================================
class MIL_AgentTypeInjuredHumanPion : public MIL_AgentTypePion
{
public:
    MIL_AgentTypeInjuredHumanPion( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypeInjuredHumanPion();

    //! @name Instanciation
    //@{
    virtual MIL_AgentPion* InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis ) const;
    virtual MIL_AgentPion* InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name ) const;
    //@}

public:
    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );
};

#endif // __MIL_AgentTypeInjuredHumanPion_h_
