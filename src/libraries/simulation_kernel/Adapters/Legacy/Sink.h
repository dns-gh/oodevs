// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_LEGACY_SINK_H
#define SWORD_LEGACY_SINK_H

#include "Adapters/Sink_ABC.h"
#include <memory>

namespace propagation
{
    class ElevationGetter_ABC;
}

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;
class MIL_IDManager;
class MissionController_ABC;

namespace sword
{
    class DEC_Logger;
    class RoleExtender_ABC;
    class SinkRoleExtender;

namespace legacy
{
// =============================================================================
/** @class  Sink
    @brief  Sink
*/
// Created: SLI 2012-01-13
// =============================================================================
class Sink : public Sink_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Sink( MIL_IDManager& idManager, MissionController_ABC& missionController,
                unsigned int gcPause, unsigned int gcMult, bool logEnabled,
                const boost::shared_ptr< const TER_World >& world );
    virtual ~Sink();
    //@}

    //! @name Factory
    //@{
    virtual void Clean();
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, sword::RoleExtender_ABC* ext );

    virtual std::auto_ptr< propagation::FloodModel_ABC > CreateFloodModel() const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< Sink_ABC >( *this );
    }
    template< typename Archive > friend  void save_construct_data( Archive& archive, const sword::legacy::Sink* sink, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, sword::legacy::Sink* sink, const unsigned int /*version*/ );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateRoles( SinkRoleExtender& ext );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< propagation::ElevationGetter_ABC > pElevation_;
    const unsigned int gcPause_;
    const unsigned int gcMult_;
    std::auto_ptr< sword::DEC_Logger > decLogger_;
    const boost::shared_ptr< const TER_World > world_;
    MIL_IDManager& idManager_;
    MissionController_ABC& missionController_;
    //@}
};
}
}

BOOST_CLASS_EXPORT_KEY( sword::legacy::Sink )

#endif // SWORD_LEGACY_SINK_H
