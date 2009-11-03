// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObject_h_
#define __UrbanObject_h_

#include "game_asn/Simulation.h"
#include "Localisation.h"
#include "clients_kernel/Object_ABC.h"
#include "SimpleEntity.h"

#include <boost/ptr_container/ptr_vector.hpp>

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
class UrbanObject : public SimpleEntity<>
{
public:
    //! @name Constructors/Destructor
    //@{
    UrbanObject( Model& model, const ASN1T_MsgUrbanCreation& msg );
    virtual ~UrbanObject();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor ) const;

    void Update             ( const ASN1T_MsgUrbanCreation& msg  );
    virtual void Display    ( kernel::Displayer_ABC& displayer  ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanObject( const UrbanObject& );            //!< Copy constructor
    UrbanObject& operator=( const UrbanObject& ); //!< Assignement operator
    //@}

public:    

    Model&                 model_;
    const std::string      strName_;
    Localisation           localisation_;
    unsigned long          id_;

};

}

#endif // __Object_h_
