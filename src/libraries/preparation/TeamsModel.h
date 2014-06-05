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

#include <tools/Resolver.h>
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xostream;
    class xistream;
}

namespace kernel
{
    class Team_ABC;
    class Controllers;
    class Entity_ABC;
}

class Model;
class TeamFactory_ABC;
class KnowledgeGroupsModel;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: SBO 2006-08-30
// =============================================================================
class TeamsModel : public tools::Resolver< kernel::Team_ABC >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::Team_ABC >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamsModel( kernel::Controllers& controllers, TeamFactory_ABC& factory, KnowledgeGroupsModel& knowledgeGroupsModel );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xistream& xis, Model& model );
    void Purge();
    void CreateTeam();
    kernel::Team_ABC* FindTeam( const QString& name ) const;
    kernel::Team_ABC* FindTeam( unsigned int id ) const;
    const kernel::Team_ABC& GetNoSideTeam() const;
    void Serialize( xml::xostream& xos ) const;
    tools::Iterator< const kernel::Entity_ABC& > CreateEntityIterator() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    void ReadTeam( const std::string& tag, xml::xistream& xis, Model& model );
    void ReadDiplomacy( xml::xistream& xis );
    void ReadLogistic( xml::xistream& xis, Model& model );
    void ReadLogisticLink( xml::xistream& xis, Model& model, kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    TeamFactory_ABC& factory_;
    KnowledgeGroupsModel& knowledgeGroupsModel_;
    std::unique_ptr< kernel::Team_ABC > noSideTeam_;
    //@}
};

#endif // __TeamsModel_h_
