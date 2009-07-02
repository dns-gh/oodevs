// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Convoy/MIL_AgentPionLOGConvoy.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentPionLOGConvoy.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOGConvoy_h_
#define __MIL_AgentPionLOGConvoy_h_

#include "MIL.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

namespace xml
{
    class xostream;
    class xistream;
}

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOGConvoy : public MIL_AgentPionLOG_ABC
{

public:
             MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOGConvoy();
    virtual ~MIL_AgentPionLOGConvoy();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    virtual void WriteODB( xml::xostream& xos ) const;
    //@}
};

#endif // __MIL_AgentPionLOGConvoy_h_
