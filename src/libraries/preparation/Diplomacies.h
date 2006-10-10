// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Diplomacies_h_
#define __Diplomacies_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Diplomacy;

// =============================================================================
/** @class  Diplomacies
    @brief  Diplomacies
*/
// Created: AGE 2006-02-14
// =============================================================================
class Diplomacies : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Team_ABC >& resolver, const kernel::Team_ABC& team );
    virtual ~Diplomacies();
    //@}

    //! @name Operations
    //@{
    const Diplomacy& GetDiplomacy( const kernel::Team_ABC& team );
    void SetDiplomacy( const kernel::Team_ABC& team, const Diplomacy& diplomacy );
    void Load( xml::xistream& xis );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Diplomacies( const Diplomacies& );            //!< Copy constructor
    Diplomacies& operator=( const Diplomacies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadRelationship( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< const Diplomacies*, Diplomacy > T_Diplomacies;
    typedef T_Diplomacies::const_iterator           CIT_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Team_ABC >& resolver_;
    T_Diplomacies diplomacies_;
    const kernel::Team_ABC& team_;
    //@}
};

#endif // __Diplomacies_h_
