// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamsModel_h_
#define __TeamsModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"

namespace xml
{
    class xostream;
    class xistream;
}

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Controllers;
    class Entity_ABC;
    class ObjectType;
    class Location_ABC;
    class Object_ABC;
}

class TeamFactory_ABC;
class Model;
class ModelChecker_ABC;
struct Enum_ObstacleType;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: SBO 2006-08-30
// =============================================================================
class TeamsModel : public tools::Resolver< kernel::Team_ABC >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamsModel( kernel::Controllers& controllers, TeamFactory_ABC& factory );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xistream& xis, Model& model );
    void Purge();
    void CreateTeam();
    void CreateKnowledgeGroup( const kernel::Team_ABC& team );
    kernel::Object_ABC* CreateObject( const kernel::Team_ABC& team, const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location );

    kernel::Team_ABC* FindTeam( const QString& name ) const;
    kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( const unsigned long& id ) const;

    void Serialize( xml::xostream& xos ) const;

    tools::Iterator< const kernel::Entity_ABC& > CreateEntityIterator() const;
    bool CheckValidity( ModelChecker_ABC& checker ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamsModel( const TeamsModel& );            //!< Copy constructor
    TeamsModel& operator=( const TeamsModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    void ReadTeam( xml::xistream& xis, Model& model );
    void ReadDiplomacy( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    TeamFactory_ABC& factory_;
    //@}
};

#endif // __TeamsModel_h_
