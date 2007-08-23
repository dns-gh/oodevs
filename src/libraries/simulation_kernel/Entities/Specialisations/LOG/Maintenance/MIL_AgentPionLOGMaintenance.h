// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Maintenance/MIL_AgentPionLOGMaintenance.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentPionLOGMaintenance.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOGMaintenance_h_
#define __MIL_AgentPionLOGMaintenance_h_

#include "MIL.h"

#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOGMaintenance : public MIL_AgentPionLOG_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentPionLOGMaintenance )

public:
    MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOGMaintenance();
    virtual ~MIL_AgentPionLOGMaintenance();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
};

#include "MIL_AgentPionLOGMaintenance.inl"

#endif // __MIL_AgentPionLOGMaintenance_h_
