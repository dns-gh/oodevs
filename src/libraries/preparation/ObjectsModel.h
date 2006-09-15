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

#include "clients_kernel/Resolver.h"

class ObjectFactory_ABC;

namespace kernel
{
    class Object_ABC;
    class ObjectType;
    class Team_ABC;
    class Location_ABC;
}

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class ObjectsModel : public kernel::Resolver< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectsModel( ObjectFactory_ABC& objectFactory );
    virtual ~ObjectsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    kernel::Object_ABC* CreateObject( kernel::ObjectType& type, kernel::Team_ABC& team, const kernel::Location_ABC& location );
    kernel::Object_ABC& GetObject( unsigned long id );
    void DeleteObject( unsigned long id );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectsModel( const ObjectsModel& );            //!< Copy constructor
    ObjectsModel& operator=( const ObjectsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ObjectFactory_ABC& objectFactory_;
    //@}
};

#endif // __ObjectsModel_h_
