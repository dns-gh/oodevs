// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_UrbanObject_h_
#define __gui_UrbanObject_h_

#include "clients_kernel/UrbanObject_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class UrbanDisplayOptions;

// =============================================================================
/** @class  UrbanObject
    @brief  Urban object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject : public kernel::UrbanObject_ABC
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< UrbanDisplayOptions >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( kernel::Controllers& controllers, const std::string& name, unsigned int id,
                                 const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations,
                                 UrbanDisplayOptions& options);
             UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, const kernel::ObjectType& type,
                                 const kernel::AccommodationTypes& accommodations, UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateColor();
    virtual void NotifyUpdated( const UrbanDisplayOptions& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UrbanDisplayOptions& options_;
    //@}
};

}

#endif // __gui_UrbanObject_h_
