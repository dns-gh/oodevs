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

#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
    class xostream;
}

class TeamKarmas;

// =============================================================================
/** @class  Diplomacies
    @brief  Diplomacies
*/
// Created: AGE 2006-02-14
// =============================================================================
class Diplomacies : public kernel::Diplomacies_ABC
                  , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, const kernel::Team_ABC& team, kernel::PropertiesDictionary& dico, TeamKarmas& karmas );
             Diplomacies( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::Team_ABC >& resolver, const kernel::Team_ABC& team, kernel::PropertiesDictionary& dico, TeamKarmas& karmas );
    virtual ~Diplomacies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Karma& GetDiplomacy( const kernel::Entity_ABC& entity ) const;
    virtual const kernel::Karma& GetKarma() const;
    void SetDiplomacy( const kernel::Team_ABC& team, const kernel::Karma& diplomacy );
    void Load( xml::xistream& xis );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Diplomacies( const Diplomacies& );            //!< Copy constructor
    Diplomacies& operator=( const Diplomacies& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    void ReadRelationship( xml::xistream& xis );
    void SetKarma( const kernel::TeamKarma& karma );
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Diplomacies_ABC*, kernel::Karma > T_Diplomacies;
    typedef T_Diplomacies::const_iterator                           CIT_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Team_ABC >& resolver_;
    T_Diplomacies diplomacies_;
    const kernel::Team_ABC& team_;
    kernel::TeamKarma karma_;
    //@}
};

#endif // __Diplomacies_h_
