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
             MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis );
             MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type, MissionController_ABC& controller );
    virtual ~MIL_AgentPionLOGMedical();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}
private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AgentPionLOGMedical* pion, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AgentPionLOGMedical* pion, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentPionLOGMedical )

#endif // __MIL_AgentPionLOGMedical_h_
