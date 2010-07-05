// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_h_
#define __ObjectFactory_h_

#include "ObjectFactory_ABC.h"


namespace Common
{
    class MsgObjectAttributes;
}

namespace kernel
{
    class Controllers;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ObjectFactory
    @brief  Object factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class ObjectFactory : public ObjectFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Object_ABC* Create( const MsgsSimToClient::MsgObjectCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectFactory( const ObjectFactory& );            //!< Copy constructor
    ObjectFactory& operator=( const ObjectFactory& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void Register( kernel::Object_ABC& result, const Common::MsgObjectAttributes& attributes ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectFactory_h_
