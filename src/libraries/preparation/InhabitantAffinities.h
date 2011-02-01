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


class AffinityFloat
{
public:
    //! @name Constructors/Destructor
    //@{
             AffinityFloat() : value_( 0.f ) {}
             AffinityFloat( float value ) : value_( value ) {}
    virtual ~AffinityFloat() {}
    //@}

    //! @name Operations
    //@{
    const float& Value() const { return value_; }
    float& Value() { return value_; }
    void setValue( float value ) { value_ = value; }
    //@}

public:
    //! @name Copy/Assignment
    //@{
    AffinityFloat( const AffinityFloat& other ) : value_( other.Value() ) {}            //!< Copy constructor
    AffinityFloat& operator=( const AffinityFloat& other ) { value_ = other.Value(); return *this; }  //!< Assignment operator
    AffinityFloat& operator=( const float& value ) { value_ = value; return *this; }  //!< Assignment operator
    //@}

private:
    float value_;
};



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
    //! @name Types
    //@{
    typedef std::map< unsigned long, AffinityFloat >     T_Affinities;
    typedef T_Affinities::iterator                      IT_Affinities;
    typedef T_Affinities::const_iterator               CIT_Affinities;

    typedef std::map< unsigned long, std::string >       T_TeamMap;
    typedef T_TeamMap::iterator                         IT_TeamMap;
    typedef T_TeamMap::const_iterator                  CIT_TeamMap;
    //@}

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
