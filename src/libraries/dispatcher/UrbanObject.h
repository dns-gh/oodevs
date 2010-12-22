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
#include "UrbanObject_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace sword
{
    class UrbanCreation;
    class UrbanUpdate;
    class UrbanAttributes;
}

namespace kernel
{
    class ModelVisitor_ABC;
    class Displayer_ABC;
}
namespace dispatcher
{
    class Model_ABC;
    class UrbanObjectAttribute_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  UrbanObject
    @brief  UrbanObject
*/
// Created: SLG 2009-12-03
// =============================================================================
    class UrbanObject : public UrbanObject_ABC
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::UrbanUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( Model_ABC& model, const sword::UrbanCreation& msg );
    virtual ~UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void DoUpdate( const sword::UrbanUpdate& msg );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanObject( const UrbanObject& );            //!< Copy constructor
    UrbanObject& operator=( const UrbanObject& ); //!< Assignment operator
    //@}

    //! @name Attributes
    //@{
    void Initialize( const sword::UrbanAttributes& attributes );
    void AddAttribute( UrbanObjectAttribute_ABC* attribute );
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< UrbanObjectAttribute_ABC > T_UrbanObjectAttributes;
    //@}

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned localisationPresent : 1;
        unsigned attributesPresent : 1;
    };

private:
    //! @name Member data
    //@{
    const std::string strName_;
    Localisation localisation_;
    bool hasInfrastructures_;
    bool hasResourceNetwork_;
    bool hasStructure_;
    T_UrbanObjectAttributes attributes_;
    T_Optionals optionals_;
    //@}
};

}

#endif // __UrbanObject_h_
