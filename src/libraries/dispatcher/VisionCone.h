// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __VisionCone_h_
#define __VisionCone_h_

#include "SimpleEntity.h"
#include "protocol/SimulationSenders.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace sword
{
    class UnitVisionCones;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  VisionCone
    @brief  Vision cone
*/
// Created: MCO 2014-06-25
// =============================================================================
class VisionCone : public SimpleEntity<>,
                   public kernel::Extension_ABC,
                   public kernel::Updatable_ABC< sword::UnitVisionCones >
{
public:
    //! @name Constructors/Destructor
    //@{
             VisionCone( Model& model, const sword::UnitVisionCones& msg );
    virtual ~VisionCone();
    //@}

    //! @name Operations
    //@{
    void DoUpdate( const sword::UnitVisionCones& msg );

    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Member data
    //@{
    sword::UnitVisionCones msg_;
    //@}
};

}

#endif // __VisionCone_h_
