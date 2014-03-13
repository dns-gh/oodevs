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
public:
             MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type, MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type, MissionController_ABC& controller );
    virtual ~MIL_AgentPionLOGSupply();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AgentPionLOGSupply* pion, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AgentPionLOGSupply* pion, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( MIL_AgentPionLOGSupply )

#endif // __MIL_AgentPionLOGSupply_h_
