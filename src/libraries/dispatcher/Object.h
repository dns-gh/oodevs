 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "Localisation.h"
#include "Object_ABC.h"
#include "ObjectAttributeContainer.h"
#include "tools/Resolver.h"

namespace sword
{
    class ObjectCreation;
    class ObjectUpdate;
}

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: NLD 2006-09-19
// =============================================================================
class Object : public Object_ABC
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< sword::ObjectUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( Model_ABC& model, const sword::ObjectCreation& msg, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& msg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual const kernel::ObjectType& GetType() const;
    virtual const Team_ABC& GetTeam() const;
    virtual std::string GetSymbol() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

private:

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned localisationPresent : 1;
    };
    //@}

private:
    const kernel::ObjectType& type_;
    Localisation localisation_;
    Team_ABC& side_;
    ObjectAttributeContainer attributes_;
    T_Optionals optionals_;
};

}

#endif // __Object_h_
