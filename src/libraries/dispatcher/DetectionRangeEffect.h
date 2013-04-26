// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __DetectionRangeEffect_h_
#define __DetectionRangeEffect_h_

#include "SimpleEntity.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace sword
{
    class IndirectFirePerception;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Model;

// =============================================================================
/** @class  DetectionRangeEffect
    @brief  DetectionRangeEffect
*/
// Created: JSR 2013-04-26
// =============================================================================
class DetectionRangeEffect : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionRangeEffect( Model& model, const sword::IndirectFirePerception& message );
    virtual ~DetectionRangeEffect();
    //@}

    //! @name Operations
    //@{
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< unsigned int > perceivers_;
    unsigned int ammunition_;
    std::vector< unsigned int > fireEffects_;
    //@}
};

}

#endif // __DetectionRangeEffect_h_
