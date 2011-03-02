// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantAffinities_h_
#define __InhabitantAffinities_h_

#include "Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Inhabitant_ABC;
    class PropertiesDictionary;
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  InhabitantAffinities
    @brief  InhabitantAffinities
*/
// Created: ABR 2011-01-27
// =============================================================================
class InhabitantAffinities : public kernel::Extension_ABC
                           , public kernel::Serializable_ABC
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantAffinities( kernel::Controllers& controllers, Model& model, const kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dictionary );
             InhabitantAffinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model, const kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dico );
    virtual ~InhabitantAffinities();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyUpdated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantAffinities( const InhabitantAffinities& );            //!< Copy constructor
    InhabitantAffinities& operator=( const InhabitantAffinities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void    InitializeAffinities();
    bool    VerifyAffinitiesContent() const;
    QString GetTeamNameFromId( unsigned long id ) const;
    void    UpdateDictionary();
    void    ReadAffinity( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, InhabitantAffinity > T_Affinities;
    typedef T_Affinities::const_iterator                CIT_Affinities;

    typedef std::map< unsigned long, std::string > T_TeamMap;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&            controllers_;
    Model&                          model_;
    const kernel::Inhabitant_ABC&   inhabitant_;
    kernel::PropertiesDictionary&   dictionary_;
    T_Affinities                    affinities_;
    T_TeamMap                       knownTeams_;
    //@}
};

#endif // __InhabitantAffinities_h_
