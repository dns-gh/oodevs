// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Tc2/MIL_AgentPionLOGTC2.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentPionLOGTC2.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPionLOGTC2_h_
#define __MIL_AgentPionLOGTC2_h_

#include "MIL.h"

#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

// =============================================================================
// @class  MIL_AgentPionType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPionLOGTC2 : public MIL_AgentPionLOG_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentPionLOGTC2 )

public:
             MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, MIL_InputArchive& archive );
             MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition );
             MIL_AgentPionLOGTC2();
    virtual ~MIL_AgentPionLOGTC2();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
};

#include "MIL_AgentPionLOGTC2.inl"

#endif // __MIL_AgentPionLOGTC2_h_
