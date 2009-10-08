// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionConesToggler_h_
#define __VisionConesToggler_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class OptionVariant;
}
class Publisher_ABC;
class Services;

// =============================================================================
/** @class  VisionConesToggler
    @brief  VisionConesToggler
*/
// Created: AGE 2007-07-11
// =============================================================================
class VisionConesToggler : public QObject
                         , public tools::Observer_ABC
                         , public kernel::OptionsObserver_ABC
                         , public kernel::ElementObserver_ABC< kernel::Profile_ABC >
                         , public kernel::ElementObserver_ABC< Services >
{

public:
    //! @name Constructors/Destructor
    //@{
             VisionConesToggler( kernel::Controllers& controllers, Publisher_ABC& publisher, QObject* parent );
    virtual ~VisionConesToggler();
    //@}

    //! @name Operations
    //@{
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void NotifyUpdated( const kernel::Profile_ABC& );
    virtual void NotifyUpdated( const Services& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    VisionConesToggler( const VisionConesToggler& );            //!< Copy constructor
    VisionConesToggler& operator=( const VisionConesToggler& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ToggleVisionCones();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    bool displayCones_, displaySurfaces_, displayFog_;
    bool simulation_;
    //@}
};

#endif // __VisionConesToggler_h_
