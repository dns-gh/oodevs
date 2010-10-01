// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
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
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis );
             MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories );
    virtual ~MIL_AgentInjuredHumanPion();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    private:
    //! @name Serialization
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ );
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentInjuredHumanPion )

#endif // __MIL_AgentInjuredHumanPion_h_
