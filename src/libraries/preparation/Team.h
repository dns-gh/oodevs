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
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
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

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
           , public kernel::Extension_ABC
           , public kernel::Serializable_ABC
           , public kernel::Resolver< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controller& controller, KnowledgeGroupFactory_ABC& kgFactory, ObjectFactory_ABC& objectFactory, IdManager& idManager );
             Team( xml::xistream& xis, kernel::Controller& controller, KnowledgeGroupFactory_ABC& factory, ObjectFactory_ABC& objectFactory, IdManager& idManager );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    void CreateKnowledgeGroup();
    void CreateKnowledgeGroup( xml::xistream& xis );
    kernel::Object_ABC* CreateObject( const kernel::ObjectType& type, const kernel::Location_ABC& location );
    void CreateObject( xml::xistream& xis );
    void Rename( const QString& name );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static unsigned long ReadId  ( xml::xistream& xis );
    static QString       ReadName( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    KnowledgeGroupFactory_ABC& kgFactory_;
    ObjectFactory_ABC& objectFactory_;
    //@}
};

#endif // __Team_h_
