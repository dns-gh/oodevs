// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Connector_LocalizedString_h_
#define __ADN_Connector_LocalizedString_h_

#include "ADN_Connector_String.h"
#include "ADN_LocalizedTypes.h"

// =============================================================================
/** @class  ADN_Connector_LocalizedString
    @brief  ADN_Connector_LocalizedString
*/
// Created: ABR 2013-07-15
// =============================================================================
template< typename T >
class ADN_Connector_LocalizedString : public ADN_Connector_String< T >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Connector_LocalizedString( T* gfx );
    virtual ~ADN_Connector_LocalizedString();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SetDataPrivate(void *data);
    virtual void ConnectPrivateSub( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_ABC* pTarget );
    //@}

};

#include "ADN_Connector_LocalizedString.inl"

#endif // __ADN_Connector_LocalizedString_h_
