// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BmlExtension_ABC_h_
#define __BmlExtension_ABC_h_

#include "clients_kernel/Extension_ABC.h"

namespace plugins
{
namespace xmliaExport
{

// =============================================================================
/** @class  XmliaExportExtension_ABC
    @brief  XmliaExportExtension_ABC
*/
// Created: SBO 2008-02-29
// =============================================================================
class XmliaExportExtension_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaExportExtension_ABC() {}
    virtual ~XmliaExportExtension_ABC() {}
    //@}
};

}
}

#endif // __XmliaExportExtension_ABC_h_
