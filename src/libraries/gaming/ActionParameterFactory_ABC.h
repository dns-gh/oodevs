// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_ABC_h_
#define __ActionParameterFactory_ABC_h_

#include "game_asn/Asn.h"

class ActionParameter_ABC;

// =============================================================================
/** @class  ActionParameterFactory_ABC
    @brief  ActionParameterFactory_ABC
*/
// Created: SBO 2007-03-19
// =============================================================================
class ActionParameterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFactory_ABC() {}
    virtual ~ActionParameterFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual ActionParameter_ABC* CreateParameter( const QString& name, const ASN1T_MissionParameter& parameter ) const = 0;
    virtual ActionParameter_ABC* CreateParameter( const QString& name, const ASN1T_Line& parameter ) const = 0;
    virtual ActionParameter_ABC* CreateParameter( const QString& name, const ASN1T_LimasOrder& parameter ) const = 0;
    virtual ActionParameter_ABC* CreateParameter( const QString& name, const ASN1T_Direction& parameter ) const = 0;
    //@}
};

#endif // __ActionParameterFactory_ABC_h_
