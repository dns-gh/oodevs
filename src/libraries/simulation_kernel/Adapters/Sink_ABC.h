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

#include "AgentFactory_ABC.h"
#include "FloodModelFactory_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_ObjectManager;

namespace core
{
    class Model;
}

namespace sword
{
// =============================================================================
/** @class  Sink_ABC
    @brief  Sink base class
*/
// Created: SBO 2011-06-06
// =============================================================================
class Sink_ABC : public AgentFactory_ABC
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
    virtual void ExecutePerceptions() = 0;
    virtual void ExecuteCommands() = 0;
    virtual void ApplyEffects() = 0;
    virtual void UpdateModel( unsigned int tick, int duration, const MIL_ObjectManager& objects ) = 0;
    //@}

    //! @name Commands
    //@{
    virtual std::size_t StartCommand( const std::string& type, const core::Model& parameters ) = 0;
    virtual void StopCommand( std::size_t command ) = 0;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< AgentFactory_ABC >( *this )
                & boost::serialization::base_object< FloodModelFactory_ABC >( *this );
    }
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::Sink_ABC )

#endif // SWORD_SINK_ABC_H
