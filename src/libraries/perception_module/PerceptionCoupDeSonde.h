// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_COUP_DE_SONDE_H
#define SWORD_PERCEPTION_PERCEPTION_COUP_DE_SONDE_H

#include "Perception_ABC.h"

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PerceptionCoupDeSonde : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionCoupDeSonde( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionCoupDeSonde();
    //@}

    //! @name Execution
    //@{
    virtual void                   ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    virtual const PerceptionLevel& ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const;
    virtual const PerceptionLevel& ComputeAgent( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const;
    //@}

private:
    //! @name Member data
    //@{
    PerceptionObserver_ABC& observer_;
    const double rLength_;
    const double rWidth_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_COUP_DE_SONDE_H
