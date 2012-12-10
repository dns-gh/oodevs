// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultFactory_h_
#define __FireResultFactory_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class UnitFireDamages;
    class CrowdFireDamages;
    class Explosion;
    class StopUnitFire;
    class StopCrowdFire;
}

class AgentFireResult;
class PopulationFireResult;
class Model;
class Simulation;

// =============================================================================
/** @class  FireResultFactory
    @brief  FireResultFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultFactory : private::boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FireResultFactory( Model& model, const Simulation& simulation );
    virtual ~FireResultFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentFireResult*      CreateFireResult( const sword::UnitFireDamages& message, const kernel::Entity_ABC* firer );
    virtual PopulationFireResult* CreateFireResult( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer );
    const kernel::Entity_ABC*     GetFirer( const sword::Explosion& message ) const;
    const kernel::Entity_ABC*     GetFirer( const sword::StopUnitFire& message ) const;
    const kernel::Entity_ABC*     GetFirer( const sword::StopCrowdFire& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    const Simulation& simulation_;
    //@}
};

#endif // __FireResultFactory_h_
