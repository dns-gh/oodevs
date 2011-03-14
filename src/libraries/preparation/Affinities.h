// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Affinities_h_
#define __Affinities_h_

#include "Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class entity_ABC;
    class PropertiesDictionary;
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  Affinities
    @brief  Affinities
*/
// Created: ABR 2011-01-27
// =============================================================================
class Affinities : public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Affinities( kernel::Controllers& controllers, Model& model, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary );
             Affinities( xml::xistream& xis, kernel::Controllers& controllers, Model& model, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico );
    virtual ~Affinities();
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
    Affinities( const Affinities& );            //!< Copy constructor
    Affinities& operator=( const Affinities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeAffinities();
    bool VerifyAffinitiesContent() const;
    QString GetTeamNameFromId( unsigned long id ) const;
    void UpdateDictionary();
    void ReadAffinity( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, EntityAffinity > T_Affinities;
    typedef T_Affinities::const_iterator            CIT_Affinities;

    typedef std::map< unsigned long, std::string > T_TeamMap;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const kernel::Entity_ABC& entity_;
    kernel::PropertiesDictionary& dictionary_;
    T_Affinities affinities_;
    T_TeamMap knownTeams_;
    //@}
};

#endif // __Affinities_h_
