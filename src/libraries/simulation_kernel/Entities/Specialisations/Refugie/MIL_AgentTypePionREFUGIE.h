// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AgentTypePionREFUGIE.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionREFUGIE.h $
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionREFUGIE_h_
#define __MIL_AgentTypePionREFUGIE_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentTypePionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentTypePionREFUGIE : public MIL_AgentTypePion
{

public:
             MIL_AgentTypePionREFUGIE( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AgentTypePionREFUGIE();

    //! @name Accessors
    //@{
    virtual bool IsRefugee() const;
    //@}

    static const MIL_AgentTypePion* Create( const std::string& strName, xml::xistream& xis );
};

#endif // __MIL_AgentTypePionREFUGIE_h_
