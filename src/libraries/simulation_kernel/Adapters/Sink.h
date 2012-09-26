// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef SWORD_SINK_H
#define SWORD_SINK_H

#include "Sink_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

class PopulationFactory_ABC;

namespace core
{
    class EventListener_ABC;
    class Logger_ABC;
    class Model;
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  Sink
    @brief  Sink
*/
// Created: SBO 2011-06-06
// =============================================================================
class Sink : public Sink_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Sink( AgentFactory_ABC& agentFactory, const PopulationFactory_ABC& populationFactory, unsigned int gcPause, unsigned int gcMult,
                   const std::vector< unsigned int >& dangerousObjects );
    virtual ~Sink();
    //@}

    //! @name Operations
    //@{
    virtual void ExecuteCommands();
    virtual void ApplyEffects();
    virtual void ExecutePerceptions();
    virtual void UpdateModel( unsigned int tick, int duration );

    void PostCommand( const std::string& type, const core::Model& parameters );
    virtual std::size_t StartCommand( const std::string& type, const core::Model& parameters );
    void PauseCommand( std::size_t command );
    void ResumeCommand( std::size_t command );
    virtual void StopCommand( std::size_t command );
    //@}

    //! @name Factory
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name );

    virtual std::auto_ptr< flood::FloodModel_ABC > CreateFloodModel() const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< Sink_ABC >( *this );
    }
    template< typename Archive > friend void save_construct_data( Archive& archive, const Sink* sink, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, Sink* sink, const unsigned int /*version*/ );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    Sink( AgentFactory_ABC& agentFactory, const PopulationFactory_ABC& populationFactory, std::auto_ptr< core::Model > model, unsigned int gcPause, unsigned int gcMult,
          const std::vector< unsigned int >& dangerousObjects );
    //@}

    //! @name Helpers
    //@{
    void Initialize();

    MIL_AgentPion& Configure( MIL_AgentPion& pion, const MT_Vector2D& position );
    //@}

private:
    //! @name Member data
    //@{
    AgentFactory_ABC& agentFactory_;
    const PopulationFactory_ABC& populationFactory_;
    const unsigned int gcPause_;
    const unsigned int gcMult_;
    const std::vector< unsigned int > dangerousObjects_;
    xml::xistringstream modules_;
    std::auto_ptr< core::Logger_ABC > logger_;
    std::auto_ptr< core::Model > model_;
    std::auto_ptr< core::Facade > facade_;
    boost::ptr_vector< core::EventListener_ABC > listeners_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::Sink )

#endif // SWORD_SINK_H
