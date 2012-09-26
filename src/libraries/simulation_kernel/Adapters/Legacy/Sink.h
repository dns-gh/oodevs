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

namespace
{
    class ElevationGetter;
}

namespace sword
{
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
             Sink( AgentFactory_ABC& factory, unsigned int gcPause, unsigned int gcMult );
    virtual ~Sink();
    //@}

    //! @name Operations
    //@{
    virtual void ExecutePerceptions();
    virtual void ExecuteCommands();
    virtual void ApplyEffects();
    virtual void UpdateModel( unsigned int tick, int duration, const MIL_ObjectManager& objects );
    virtual void UpdateUrbanModel( const MIL_UrbanCache& cache );
    //@}

    //! @name Commands
    //@{
    virtual std::size_t StartCommand( const std::string& type, const core::Model& parameters );
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
    template< typename Archive > friend  void save_construct_data( Archive& archive, const sword::legacy::Sink* sink, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, sword::legacy::Sink* sink, const unsigned int /*version*/ );
    //@}

private:
    //! @name Helpers
    //@{
    MIL_AgentPion& Configure( MIL_AgentPion& pion );

    void Initialize( MIL_AgentPion& pion, const MT_Vector2D& vPosition );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ElevationGetter > pElevation_;
    AgentFactory_ABC& factory_;
    const unsigned int gcPause_;
    unsigned int gcMult_;
    //@}
};

}
}

BOOST_CLASS_EXPORT_KEY( sword::legacy::Sink )

#endif // SWORD_LEGACY_SINK_H
