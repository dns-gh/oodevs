// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Architecture_h_
#define __Architecture_h_

#include "clients_kernel/Architecture.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture : public kernel::Architecture
                   , public tools::Observer_ABC
                   , public kernel::ModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture( const kernel::Entity_ABC* parent, kernel::Controllers& controllers, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes );
             Architecture( kernel::Controllers& controllers, xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes );
    virtual ~Architecture();
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
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __Architecture_h_
