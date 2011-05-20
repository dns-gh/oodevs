// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "client_proxy/SwordMessageHandler_ABC.h"
#include <map>
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

class Agent;
class Automat;
class BoundaryLimit;
class CWISEDriver;
class CWISEValueUnion;
class Drawing;
class FireEngagement;
class Formation;
class KnowledgeGroup;
class Obstacle;
class Party;
class PhaseLine;
class Simulation;
class SwordMessagePublisher_ABC;
class Weather;
class WiseEntity;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: SEB 2010-10-13
// =============================================================================
class Model : public SwordMessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Model( CWISEDriver& driver, const WISE_HANDLE& database, SwordMessagePublisher_ABC& publisher );
    virtual ~Model();
    //@}

    //! @name Simulation message handlers
    //@{
    virtual void OnReceiveMessage( const sword::SimToClient& message );
    virtual void OnReceiveMessage( const sword::MessengerToClient& message );
    virtual void OnReceiveMessage( const sword::AuthenticationToClient& message );
    //@}

    //! @name Driver message handlers
    //@{
    void OnReceiveEvent( const WISE_HANDLE& handle );
    void OnReceiveUpdate( const WISE_HANDLE& object, const WISE_HANDLE& attribute, const CWISEValueUnion& value );
    //@}

    //! @name Resolvers
    //@{
    const Party* ResolveParty( const unsigned long& id ) const;
    const KnowledgeGroup* ResolveKnowledgeGroup( const unsigned long& id ) const;
    const Formation* ResolveFormation( const unsigned long& id ) const;
    const Automat* ResolveAutomat( const unsigned long& id ) const;
    const Agent* ResolveAgent( const unsigned long& id ) const;
    const Obstacle* ResolveObstacle( const unsigned long& id ) const;
    const WiseEntity* ResolveEntity( const WISE_HANDLE& handle ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Reset();
    template< class Entity, class Message >
    void Create( std::map< unsigned long, Entity* >& entities, const Message& message );
    template< class Event, class Message >
    void CreateEvent( std::map< unsigned long, Event* >& events, const Message& message );
    template< class Entity, class Message >
    void Update( std::map< unsigned long, Entity* >& entities, unsigned long id, const Message& message );
    template< class Entity >
    void Clear( std::map< unsigned long, Entity* >& entities );
    template< class Entity >
    void Destroy( std::map< unsigned long, Entity* >& entities, unsigned long id );
    timeb GetTime();
    //@}

private:
    //! @name Member data
    //@{
    CWISEDriver& driver_;
    WISE_HANDLE database_;
    SwordMessagePublisher_ABC& publisher_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< Weather > weather_;
    std::map< unsigned long, Party* > parties_;
    std::map< unsigned long, KnowledgeGroup* > knowledgeGroups_;
    std::map< unsigned long, Formation* > formations_;
    std::map< unsigned long, Automat* > automats_;
    std::map< unsigned long, Agent* > agents_;
    std::map< unsigned long, BoundaryLimit* > boundaryLimits_;
    std::map< unsigned long, PhaseLine* > phaseLines_;
    std::map< unsigned long, Obstacle* > obstacles_;
    std::map< unsigned long, Drawing* > drawings_;
    std::map< unsigned long, FireEngagement* > fireEngagements_;
    std::map< WISE_HANDLE, WiseEntity* > entities_;
    //@}
};

#endif // __Model_h_
