// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_ALAT_H
#define SWORD_PERCEPTION_PERCEPTION_ALAT_H

#include "Perception_ABC.h"
#include <vector>

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PerceptionAlat : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionAlat( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionAlat();
    //@}

    //! @name Execution
    //@{
    virtual void ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< wrapper::View > localisations_;
    PerceptionObserver_ABC& observer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_ALAT_H
