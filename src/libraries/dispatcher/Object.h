 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "Localisation.h"
#include "Object_ABC.h"
#include "tools/Resolver.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace Common
{
    class MsgObjectAttributes;
}

namespace MsgsSimToClient
{
    class MsgObjectCreation;
    class MsgObjectUpdate;
}

namespace dispatcher
{
    class Model_ABC;
    class ObjectAttribute_ABC;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: NLD 2006-09-19
// =============================================================================
class Object : public dispatcher::Object_ABC
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( Model_ABC& model, const MsgsSimToClient::MsgObjectCreation& msg, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual const kernel::ObjectType& GetType() const;
    virtual const dispatcher::Team_ABC& GetTeam() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignement operator
    //@}

    //! @name Attributes
    //@{
    void Initialize( Model_ABC& model, const Common::MsgObjectAttributes& attribute );
    void AddAttribute( ObjectAttribute_ABC* attribute );
    //@}

private:
    //! @name Types
    //@{   
    typedef boost::ptr_vector< ObjectAttribute_ABC > T_ObjectAttributes;
    //@}

private:
    const kernel::ObjectType&    type_;

    const std::string            strName_;
          Localisation           localisation_;
          dispatcher::Team_ABC&  side_;
    
    T_ObjectAttributes           attributes_;
};

}

#endif // __Object_h_
