// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Medical/MIL_AgentInjuredHumanPion.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 3 $
// $Workfile: MIL_AgentInjuredHumanPion.h $
//
// *****************************************************************************

#ifndef __MIL_AgentInjuredHumanPion_h_
#define __MIL_AgentInjuredHumanPion_h_

#include "MIL.h"

#include "Entities/Agents/MIL_AgentPion.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AgentPionType
// Created: RFT 2004-08-03
// =============================================================================
class MIL_AgentInjuredHumanPion : public MIL_AgentPion
{
    MT_COPYNOTALLOWED( MIL_AgentInjuredHumanPion )

public:
             MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis );
             MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories );
             MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories );
    virtual ~MIL_AgentInjuredHumanPion();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    private:

    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ );
  	template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ );
    //@}
};

#endif // __MIL_AgentInjuredHumanPion_h_
