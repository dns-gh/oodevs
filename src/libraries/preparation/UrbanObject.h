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

#include "clients_gui/UrbanObject.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
}

namespace gui
{
    class UrbanDisplayOptions;
}

namespace xml
{
    class xostream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  UrbanObject
    @brief  UrbanObject
*/
// Created: ABR 2012-05-23
// =============================================================================
class UrbanObject : public gui::UrbanObject
                  , public tools::Observer_ABC
                  , public kernel::ModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( kernel::Controllers& controllers, tools::IdManager& idManager, const std::string& name, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, gui::UrbanDisplayOptions& options );
             UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, gui::UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name ModesObserver_ABC
    //@{
    virtual void NotifyModeChanged( E_Modes newMode );
    //@}
};

#endif // __UrbanObject_h_
