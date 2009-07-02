// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Medical/MIL_AgentPionLOGMedical.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentPionLOGMedical.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOGMedical_h_
#define __MIL_AgentPionLOGMedical_h_

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
class MIL_AgentPionLOGMedical : public MIL_AgentPionLOG_ABC
{

public:
    MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOGMedical();
    virtual ~MIL_AgentPionLOGMedical();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
};

#include "MIL_AgentPionLOGMedical.inl"

#endif // __MIL_AgentPionLOGMedical_h_
