// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObject_h_
#define __UrbanObject_h_

#include "Localisation.h"
#include "SimpleEntity.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace MsgsSimToClient
{
    class MsgUrbanCreation;
    class MsgUrbanUpdate;
    class MsgUrbanAttributes;
}

namespace kernel
{
    class ModelVisitor_ABC;
    class Displayer_ABC;
}
namespace dispatcher
{
    class Model;
    class UrbanObjectAttribute_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  UrbanObject
    @brief  UrbanObject
*/
// Created: SLG 2009-12-03
// =============================================================================
class UrbanObject : public SimpleEntity<>
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( Model& model, const MsgsSimToClient::MsgUrbanCreation& msg );
    virtual ~UrbanObject();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept         ( kernel::ModelVisitor_ABC& visitor ) const;
    void Update         ( const MsgsSimToClient::MsgUrbanCreation& msg );
    void Update         ( const MsgsSimToClient::MsgUrbanUpdate& msg  );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanObject( const UrbanObject& );            //!< Copy constructor
    UrbanObject& operator=( const UrbanObject& ); //!< Assignment operator
    //@}

    //! @name Attributes
    //@{
    void Initialize( Model& model, const MsgsSimToClient::MsgUrbanAttributes& attributes );
    void AddAttribute( UrbanObjectAttribute_ABC* attribute );
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< UrbanObjectAttribute_ABC > T_UrbanObjectAttributes;
    //@}

public:

    Model&                      model_;
    const std::string           strName_;
    Localisation                localisation_;
    unsigned long               id_;
    T_UrbanObjectAttributes     attributes_;
};

}

#endif // __UrbanObject_h_
