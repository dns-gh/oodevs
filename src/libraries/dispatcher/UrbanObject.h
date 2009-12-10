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

#include "Localisation.h"
#include "SimpleEntity.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace dispatcher
{
class Model;
class UrbanObjectAttribute_ABC;
class ModelVisitor_ABC;
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
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanObject( const UrbanObject& );            //!< Copy constructor
    UrbanObject& operator=( const UrbanObject& ); //!< Assignement operator
    //@}

    //! @name Attributes
    //@{
    void Initialize( Model& model, const ASN1T_UrbanAttributes& attributes );
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
