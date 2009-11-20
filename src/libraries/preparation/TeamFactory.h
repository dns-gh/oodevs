// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"
#include "clients_kernel/KnowledgeGroupFactory_ABC.h"
#include "ObjectFactory_ABC.h"

namespace tools
{
    template< typename T > class SortedInterfaceContainer;
}

namespace kernel
{
    class Controllers;
    class PropertiesDictionary;    
    class Extension_ABC;
    class KnowledgeGroupFactory_ABC;
}

class Model;
class StaticModel;
class IdManager;
class ObjectAttributeFactory_ABC;

// =============================================================================
/** @class  TeamFactory
    @brief  TeamFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory : public TeamFactory_ABC
                  , public ObjectFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager );
    virtual ~TeamFactory();
    //@}

    //! @name 
    //@{
    void Initialize();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam();
    virtual kernel::Team_ABC* CreateTeam( xml::xistream& xis );

    virtual kernel::Object_ABC* CreateObject( const kernel::ObjectType& type, kernel::Team_ABC& team, const QString& name, const kernel::Location_ABC& location );
    virtual kernel::Object_ABC* CreateObject( xml::xistream& xis, kernel::Team_ABC& team );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamFactory( const TeamFactory& );            //!< Copy constructor
    TeamFactory& operator=( const TeamFactory& ); //!< Assignement operator
    //@}

    //! @name Helper
    //@{
    void ReadAttributes( const std::string& attr, xml::xistream& xis, tools::SortedInterfaceContainer< kernel::Extension_ABC >& result, kernel::PropertiesDictionary& dico );    
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& staticModel_;
    IdManager& idManager_;
    std::auto_ptr< ObjectAttributeFactory_ABC >   factory_;
    //@}
};

#endif // __TeamFactory_h_
