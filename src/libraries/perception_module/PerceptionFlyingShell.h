// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_FLYING_SHELL_H
#define SWORD_PERCEPTION_PERCEPTION_FLYING_SHELL_H

#include "Perception_ABC.h"

namespace xml
{
    class xistream;
}

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

// =============================================================================
// @class  PerceptionFlyingShell
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PerceptionFlyingShell : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionFlyingShell( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionFlyingShell();
    //@}

    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Execution
    //@{
    virtual void ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< wrapper::View > T_ZoneVector;
    //@}

    //! @name Add Points
    //@{
    void AddLocalisation( const wrapper::View& perception );
    //@}


private:
    //! @name Member data
    //@{
    T_ZoneVector  zones_;
    static double rRadius_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_FLYING_SHELL_H
