// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ResourceNetworks_Wizard_h_
#define __ADN_ResourceNetworks_Wizard_h_

#include "ADN_ResourceNetworks_Data.h"
#include "ADN_Wizard_ABC.h"

// =============================================================================
/** @class  ADN_ResourceNetworks_Wizard
    @brief  ADN_ResourceNetworks_Wizard
*/
// Created: JSR 2010-09-14
// =============================================================================
class ADN_ResourceNetworks_Wizard : public ADN_Wizard_ABC< ADN_ResourceNetworks_Data::ResourceNetworkInfos >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ResourceNetworks_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ResourceNetworks_Wizard();
    //@}
};

#endif // __ADN_ResourceNetworks_Wizard_h_
