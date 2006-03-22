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

#include "ASN_Types.h"
#include "Resolver.h"

class ObjectFactory_ABC;
class Object;


// =============================================================================
/** @class  ObjectsModel
    @brief  ObjectsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class ObjectsModel : public Resolver< Object >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectsModel( ObjectFactory_ABC& objectFactory ); 
    virtual ~ObjectsModel();
    //@}

    //! @name Operations
    //@{
    void CreateObject( const ASN1T_MsgObjectCreation& asnMsg );
    Object& GetObject( unsigned long id );
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
