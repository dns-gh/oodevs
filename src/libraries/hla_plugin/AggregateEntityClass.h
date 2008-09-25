// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AggregateEntityClass_h_
#define __AggregateEntityClass_h_

#include "ObjectClass_ABC.h"

namespace hla
{
    template< typename T > class Class;
    class Federate;
}

namespace plugins
{
namespace hla
{
    class HlaExtension_ABC;

// =============================================================================
/** @class  AggregateEntityClass
    @brief  AggregateEntityClass
*/
// Created: AGE 2008-02-22
// =============================================================================
class AggregateEntityClass : public ObjectClass_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntityClass();
    virtual ~AggregateEntityClass();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterTo( ::hla::Federate& federate );
    void Register( HlaExtension_ABC& localObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AggregateEntityClass( const AggregateEntityClass& );            //!< Copy constructor
    AggregateEntityClass& operator=( const AggregateEntityClass& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct UnitRegistration;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< UnitRegistration > registration_;
    std::auto_ptr< ::hla::Class< HlaExtension_ABC > > hlaClass_;
    //@}
};

}
}

#endif // __AggregateEntityClass_h_
