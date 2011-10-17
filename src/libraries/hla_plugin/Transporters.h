// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Transporters_h
#define plugins_hla_Transporters_h

#include "Transporters_ABC.h"
#include "AgentListener_ABC.h"
#include "EventListener_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace plugins
{
namespace hla
{
    class AgentSubject_ABC;
    class CallsignResolver_ABC;
    class SideChecker_ABC;
    class AutomatChecker_ABC;

// =============================================================================
/** @class  Transporters
    @brief  Transporters
*/
// Created: SLI 2011-10-11
// =============================================================================
class Transporters : public Transporters_ABC
                   , private AgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Transporters( AgentSubject_ABC& agentSubject, const CallsignResolver_ABC& callsignResolver,
                           const SideChecker_ABC& sideChecker, const AutomatChecker_ABC& automatChecker );
    virtual ~Transporters();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( const std::string& transportedUnitUniqueId, const geometry::Point2d& embarkmentPoint, TransportedUnitsVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol );
    virtual void SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    //@}

private:
    //! @name Types
    //@{
    struct T_Vessel : public EventListener_ABC
    {
    public:
        virtual void SpatialChanged( double latitude, double longitude, float /*altitude*/, float /*speed*/, float /*direction*/ )
        {
            position_ = geometry::Point2d( latitude, longitude );
        }
        virtual void FormationChanged( bool /*isOnRoad*/ ) {}
        virtual void EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ ) {}
        virtual void EmbarkmentChanged( bool /*mounted*/ ) {}
        geometry::Point2d position_;
        unsigned long identifier_;
    };
    typedef std::vector< boost::shared_ptr< T_Vessel > > T_Vessels;
    //@}

private:
    //! @name Member data
    //@{
    AgentSubject_ABC& agentSubject_;
    const CallsignResolver_ABC& callsignResolver_;
    const SideChecker_ABC& sideChecker_;
    const AutomatChecker_ABC& automatChecker_;
    T_Vessels vessels_;
    //@}
};

}
}

#endif // Transporters_h
