// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PeopleAffinities_h_
#define __PeopleAffinities_h_

#include "Affinities.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Team_ABC;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  PeopleAffinities
    @brief  People affinities
*/
// Created: ABR 2011-01-27
// =============================================================================
class PeopleAffinities : public Affinities
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PeopleAffinities( kernel::Controllers& controllers, Model& model, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             PeopleAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~PeopleAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void Clear();
    virtual void Add( unsigned long team, float value );
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeAffinities();
    void ReadAffinity( xml::xistream& xis );
    void AddTeam( const kernel::Team_ABC& team );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, std::string > T_Teams;
    typedef T_Teams::const_iterator              CIT_Teams;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::PropertiesDictionary& dictionary_;
    T_Teams teams_;
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __PeopleAffinities_h_
