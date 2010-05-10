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
#include "clients_kernel/Object_ABC.h"
#include "SimpleEntity.h"
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

namespace kernel
{
    class ModelVisitor_ABC;
    class ObjectType;
    class StaticModel;
    class Team_ABC;
}

namespace dispatcher
{
    class Model;
    class ObjectAttribute_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: NLD 2006-09-19
// =============================================================================
class Object : public SimpleEntity< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( Model& model, const MsgsSimToClient::MsgObjectCreation& msg, const kernel::StaticModel& staticModel );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgObjectUpdate&   msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual const kernel::ObjectType& GetType() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignement operator
    //@}

    //! @name Attributes
    //@{
    void Initialize( Model& model, const Common::MsgObjectAttributes& attribute );
    void AddAttribute( ObjectAttribute_ABC* attribute );
    //@}

private:
    //! @name Types
    //@{   
    typedef boost::ptr_vector< ObjectAttribute_ABC > T_ObjectAttributes;
    //@}

public:    
    const kernel::ObjectType&    type_;

    const std::string            strName_;
          Localisation           localisation_;
          kernel::Team_ABC&      side_;
    
    T_ObjectAttributes           attributes_;
};

}

#endif // __Object_h_
