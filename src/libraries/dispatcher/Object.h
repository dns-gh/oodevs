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

#include "game_asn/Simulation.h"
#include "Localisation.h"
#include "clients_kernel/Object_ABC.h"
#include "SimpleEntity.h"

#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
	class ObjectType;
    class Team_ABC;
}

namespace dispatcher
{
    class Model;
    class ObjectAttribute_ABC;
    class ModelVisitor_ABC;
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
             Object( Model& model, const ASN1T_MsgObjectCreation& msg );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgObjectUpdate&   msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor ) const;

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
    void Initialize( Model& model, const ASN1T_ObjectAttributes& attribute );
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
