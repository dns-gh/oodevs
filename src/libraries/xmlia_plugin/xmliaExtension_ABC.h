// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaExtension_ABC_h_
#define __XmliaExtension_ABC_h_

#include "clients_kernel/Extension_ABC.h"

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  XmliaExtension_ABC
    @brief  XmliaExtension_ABC
*/
// Created: SBO 2008-02-29
// =============================================================================
class XmliaExtension_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaExtension_ABC() {}
    virtual ~XmliaExtension_ABC() {}
    //@}
};

}
}

#endif // __XmliaExtension_ABC_h_
