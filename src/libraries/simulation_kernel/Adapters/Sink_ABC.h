// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef SWORD_SINK_ABC_H
#define SWORD_SINK_ABC_H

#include "FloodModelFactory_ABC.h"
#include <boost/serialization/export.hpp>
#include <tools/Resolver.h>

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;
class MIL_ObjectManager;
class MIL_UrbanCache;
class MIL_EffectManager;

namespace sword
{
class RoleExtender_ABC;

// =============================================================================
/** @class  Sink_ABC
    @brief  Sink base class
*/
// Created: SBO 2011-06-06
// =============================================================================
class Sink_ABC : public tools::Resolver< MIL_AgentPion >
               , public FloodModelFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Sink_ABC() {}
    virtual ~Sink_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Finalize() = 0;
    virtual void Clean() = 0;
    virtual void ExecutePerceptions() = 0;
    virtual void ExecuteCommands() = 0;
    virtual void ApplyEffects() = 0;
    virtual void NotifyEffects() = 0;
    virtual void UpdateModel( unsigned int tick, int duration, const MIL_ObjectManager& objects, const MIL_EffectManager& effects ) = 0;
    virtual void UpdateKnowledges() = 0;
    virtual void UpdateUrbanModel( const MIL_UrbanCache& cache ) = 0;
    virtual void LogProfiling() = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext ) = 0;
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, sword::RoleExtender_ABC* ext ) = 0;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< FloodModelFactory_ABC >( *this );
    }
    //@}
};
}

BOOST_CLASS_EXPORT_KEY( sword::Sink_ABC )

#endif // SWORD_SINK_ABC_H
