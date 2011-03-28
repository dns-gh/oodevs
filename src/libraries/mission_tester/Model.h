// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "client_proxy/SwordMessageHandler_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace kernel
{
    class StaticModel;
}

namespace sword
{
    class UnitCreation;
}

namespace mission_tester
{
    class Agent;
// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: PHC 2011-03-28
// =============================================================================
class Model : private boost::noncopyable
            , public SwordMessageHandler_ABC
            , public kernel::EntityResolver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Model( const kernel::StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    virtual void OnReceiveMessage( const sword::SimToClient& message );
    virtual void OnReceiveMessage( const sword::MessengerToClient& message );
    //@}

    //! @name Resolvers
    //@{
    virtual kernel::Agent_ABC* FindAgent( unsigned int id ) const;
    virtual kernel::Agent_ABC& GetAgent( unsigned int id ) const;
    virtual kernel::Automat_ABC* FindAutomat( unsigned int id ) const;
    virtual kernel::Automat_ABC& GetAutomat( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int id ) const;
    virtual kernel::Object_ABC* FindObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetObject( unsigned int id ) const;
    virtual kernel::Object_ABC* FindUrbanObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetUrbanObject( unsigned int id ) const;
    virtual kernel::Population_ABC* FindPopulation( unsigned int id ) const;
    virtual kernel::Population_ABC& GetPopulation( unsigned int id ) const;
    virtual kernel::Formation_ABC* FindFormation( unsigned int id ) const;
    virtual kernel::Formation_ABC& GetFormation( unsigned int id ) const;
    virtual kernel::Team_ABC* FindTeam( unsigned int id ) const;
    virtual kernel::Team_ABC& GetTeam( unsigned int id ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateAgent( const sword::UnitCreation& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, boost::shared_ptr< Agent > > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::StaticModel& staticModel_;
    T_Agents agents_;
    //@}
};
}

#endif // __Model_h_
