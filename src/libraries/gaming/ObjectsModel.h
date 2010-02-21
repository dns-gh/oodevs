// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsModel_h_
#define __ObjectsModel_h_


#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgObjectCreation;
}

class ObjectFactory_ABC;

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class ObjectsModel : public tools::Resolver< kernel::Object_ABC >
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

    void CreateObject( const MsgsSimToClient::MsgObjectCreation& message );
    kernel::Object_ABC& GetObject( unsigned long id );
    void DeleteObject( unsigned long id );
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
