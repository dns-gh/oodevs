// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanObject_h_
#define __UrbanObject_h_

#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/UrbanObject.h"

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
    class UrbanDisplayOptions;
}

namespace xml
{
    class xostream;
}

class IdManager;

// =============================================================================
/** @class  UrbanObject
    @brief  UrbanObject
*/
// Created: ABR 2012-05-23
// =============================================================================
class UrbanObject : public kernel::UrbanObject
                  , public kernel::Serializable_ABC
                  , public kernel::ModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( kernel::Controllers& controllers, IdManager& idManager, const std::string& name, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options );
             UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, IdManager& idManager, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, kernel::UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name ModesObserver_ABC
    //@{
    virtual void NotifyModeChanged( int newMode );
    //@}
};

#endif // __UrbanObject_h_
