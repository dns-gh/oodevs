// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsModel_h_
#define __ObjectsModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
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
    class ObjectType;
    class Location_ABC;
    class Object_ABC;
}

class Model;
class ObjectFactory_ABC;

// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: JSR 2011-02-22
// =============================================================================
class ObjectsModel : public tools::Resolver< kernel::Object_ABC >
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< kernel::Object_ABC >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectsModel( kernel::Controllers& controllers, ObjectFactory_ABC& factory );
    virtual ~ObjectsModel();
    //@}

    //! @name Operations
    //@{
    void Finalize();
    void Purge();
    kernel::Object_ABC* CreateObject( const kernel::Team_ABC& team, const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location );
    void CreateObject( xml::xistream& xis, const kernel::Team_ABC& team, std::string& loadingErrors );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Object_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ObjectFactory_ABC& factory_;
    //@}
};

#endif // __ObjectsModel_h_
