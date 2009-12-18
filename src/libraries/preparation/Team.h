// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controller;
    class ObjectType;
    class Location_ABC;
    class Object_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class KnowledgeGroupFactory_ABC;
class ObjectFactory_ABC;
class IdManager;
struct Enum_ObstacleType;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
           , public kernel::Extension_ABC
           , public kernel::Serializable_ABC
           , public tools::Resolver< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controller& controller, ObjectFactory_ABC& objectFactory, IdManager& idManager );
             Team( xml::xistream& xis, kernel::Controller& controller, ObjectFactory_ABC& objectFactory, IdManager& idManager );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    kernel::Object_ABC* CreateObject( const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location );
    void CreateObject( xml::xistream& xis );
    void Rename( const QString& name );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual const kernel::Karma& GetKarma() const;
    virtual void Register( kernel::Formation_ABC& formation );
    virtual void Remove( kernel::Formation_ABC& formation );
    virtual void Register( kernel::Population_ABC& population );
    virtual void Remove( kernel::Population_ABC& population );
    virtual void Register( kernel::Object_ABC& object );
    virtual void Remove( kernel::Object_ABC& object );
    virtual void Register( kernel::KnowledgeGroup_ABC& knGroup );
    virtual void Remove( kernel::KnowledgeGroup_ABC& knGroup );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    ObjectFactory_ABC& objectFactory_;
    //@}
};

#endif // __Team_h_
