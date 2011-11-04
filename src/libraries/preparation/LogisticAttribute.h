// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "clients_kernel/Finalizable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Controllers;
    class Displayer_ABC;
    class PropertiesDictionary;
    class Object_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticAttribute : public kernel::LogisticAttribute_ABC
                        , public kernel::Finalizable_ABC
                        , public kernel::Serializable_ABC
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                        , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute( kernel::PropertiesDictionary& dico, kernel::Controllers& controllers, const kernel::Object_ABC& object );
             LogisticAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                const tools::Resolver_ABC< kernel::Formation_ABC >& formations, kernel::PropertiesDictionary& dico,
                                kernel::Controllers& controllers, const kernel::Object_ABC& object );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void Finalize();
    //@}

    //! @name Operations
    //@{
    void SetLogisticBase( const kernel::Entity_ABC& logisticBase );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Automat_ABC& entity );
    virtual void NotifyDeleted( const kernel::Formation_ABC& entity );
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::LogisticBaseSuperior logisticBase_;
    const kernel::Object_ABC& object_;
    unsigned long idToConvert_;
    const tools::Resolver_ABC< kernel::Automat_ABC >* pAutomats_;
    const tools::Resolver_ABC< kernel::Formation_ABC >* pFormations_;
    //@}
};

#endif // __LogisticAttribute_h_
