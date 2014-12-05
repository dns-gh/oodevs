// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsModel_h_
#define __ObjectsModel_h_

#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TrackingResolver.h"

namespace sword
{
    class ObjectCreation;
    class ObjectDestruction;
    class ObjectUpdate;
}

class ObjectFactory_ABC;

// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class ObjectsModel : public tools::TrackingResolver< kernel::Object_ABC >
{
public:
             ObjectsModel( kernel::Controllers& controllers, ObjectFactory_ABC& factory );
    virtual ~ObjectsModel();

    void Purge();

    void CreateObject( const sword::ObjectCreation& message );
    void DeleteObject( const sword::ObjectDestruction& message );
    void UpdateObject( const sword::ObjectUpdate& message );

private:
    ObjectFactory_ABC& factory_;
};

#endif // __ObjectsModel_h_
