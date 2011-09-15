// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AgentAffinities_h_
#define __AgentAffinities_h_

#include "Affinities.h"
#include "clients_kernel/SubTypes.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Team_ABC;
    class Entity_ABC;
    class PropertiesDictionary;
}

class Model;

// =============================================================================
/** @class  AgentAffinities
    @brief  Agent affinities
*/
// Created: LGY 2011-03-14
// =============================================================================
class AgentAffinities : public Affinities
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentAffinities( kernel::Entity_ABC& agent, kernel::Controllers& controllers, Model& model,
                              kernel::PropertiesDictionary& dictionary, const QString& propertyName );
             AgentAffinities( xml::xistream& xis, kernel::Entity_ABC& agent, kernel::Controllers& controllers, Model& model,
                              kernel::PropertiesDictionary& dictionary, const QString& propertyName );
    virtual ~AgentAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void Add( unsigned long team, float affinity );
    virtual void Clear();
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, std::string > T_Teams;
    typedef T_Teams::const_iterator              CIT_Teams;
    //@}

    //! @name Member Data
    //@{
    void InitializeAffinities();
    void AddTeam( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Entity_ABC& agent_;
    kernel::PropertiesDictionary& dictionary_;
    QString propertyName_;
    T_Teams teams_;
    //@}
};

#endif // __AgentAffinities_h_
