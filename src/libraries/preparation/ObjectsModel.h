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

#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TrackingResolver.h"

namespace xml
{
    class xostream;
    class xistream;
}

namespace kernel
{
    class Team_ABC;
    class Entity_ABC;
    class ObjectType;
    class Location_ABC;
}

class Model;
class ObjectFactory_ABC;

// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: JSR 2011-02-22
// =============================================================================
class ObjectsModel : public tools::TrackingResolver< kernel::Object_ABC >
{
public:
             ObjectsModel( kernel::Controllers& controllers, ObjectFactory_ABC& factory, const tools::StringResolver< kernel::ObjectType >& resolver );
    virtual ~ObjectsModel();

    void Finalize();
    void Purge();

    kernel::Object_ABC* CreateObject( const kernel::Team_ABC& team, const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location );
    void CreateObject( xml::xistream& xis, const kernel::Team_ABC& team, Model& model );

private:
    ObjectFactory_ABC& factory_;
    const tools::StringResolver< kernel::ObjectType >& resolver_;
};

#endif // __ObjectsModel_h_
