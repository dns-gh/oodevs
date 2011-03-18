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
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Team_ABC;
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
             PeopleAffinities( kernel::Controllers& controllers, Model& model );
             PeopleAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model );
    virtual ~PeopleAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void Clear();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PeopleAffinities( const PeopleAffinities& );            //!< Copy constructor
    PeopleAffinities& operator=( const PeopleAffinities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeAffinities();
    void ReadAffinity( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, EntityAffinity > T_Affinities;
    typedef T_Affinities::const_iterator            CIT_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    T_Affinities PeopleAffinities_;
    //@}
};

#endif // __PeopleAffinities_h_
