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

namespace sword
{
    class DEC_Logger;
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
             Sink( AgentFactory_ABC& factory, unsigned int gcPause, unsigned int gcMult, bool logEnabled );
    virtual ~Sink();
    //@}

    //! @name Operations
    //@{
    virtual void Finalize();
    virtual void Clean();
    virtual void ExecutePerceptions();
    virtual void ExecuteCommands();
    virtual void ApplyEffects();
    virtual void NotifyEffects();
    virtual void UpdateModel( unsigned int tick, int duration, const MIL_ObjectManager& objects, const MIL_EffectManager& effects );
    virtual void UpdateKnowledges();
    virtual void UpdateUrbanModel( const MIL_UrbanCache& cache );
    virtual void LogProfiling();
    //@}

    //! @name Commands
    //@{
    virtual std::size_t StartCommand( const std::string& type, const core::Model& parameters );
    virtual void StopCommand( std::size_t command );
    //@}

    //! @name Factory
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, sword::RoleExtender_ABC* ext );
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

    //! @name Accessors
    //@{
    virtual unsigned long GetModelCount() const;
    //@}

private:
    //! @name Helpers
    //@{
    MIL_AgentPion& Configure( MIL_AgentPion& pion );
    void CreateRoles( SinkRoleExtender& ext );

    void Initialize( MIL_AgentPion& pion, const MT_Vector2D& vPosition );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< propagation::ElevationGetter_ABC > pElevation_;
    AgentFactory_ABC& factory_;
    const unsigned int gcPause_;
    const unsigned int gcMult_;
    std::auto_ptr< sword::DEC_Logger > decLogger_;
    //@}
};
}
}

BOOST_CLASS_EXPORT_KEY( sword::legacy::Sink )

#endif // SWORD_LEGACY_SINK_H
