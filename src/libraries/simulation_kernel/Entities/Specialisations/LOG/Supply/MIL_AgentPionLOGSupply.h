// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Supply/MIL_AgentPionLOGSupply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentPionLOGSupply.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOGSupply_h_
#define __MIL_AgentPionLOGSupply_h_

#include "MIL.h"

#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOGSupply : public MIL_AgentPionLOG_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentPionLOGSupply )

public:
             MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, MIL_InputArchive& archive );
             MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOGSupply();
    virtual ~MIL_AgentPionLOGSupply();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
};

#include "MIL_AgentPionLOGSupply.inl"

#endif // __MIL_AgentPionLOGSupply_h_
