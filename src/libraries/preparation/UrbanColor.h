// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColor_h_
#define __UrbanColor_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class PropertiesDictionary;
    class UrbanObject_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UrbanColor
    @brief  UrbanColor
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanColor : public kernel::UrbanColor_ABC
                 , public kernel::Serializable_ABC
                 , public tools::Observer_ABC
                 , public kernel::ModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanColor( const kernel::Entity_ABC* parent, kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary );
             UrbanColor( xml::xistream& xis, kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary );
    virtual ~UrbanColor();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name ModesObserver_ABC
    //@{
    virtual void NotifyModeChanged( int newMode );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionnary( bool readOnly );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&          controllers_;
    kernel::UrbanObject_ABC&      object_;
    kernel::PropertiesDictionary& dictionary_;
    //@}
};

#endif // __UrbanColor_h_
