// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef ADN_WeaponFilter_h
#define ADN_WeaponFilter_h

#include "ADN_Types.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Weapons_Data.h"

// =============================================================================
// @class  ADN_WeaponFilter
// @brief  ADN_WeaponFilter
//
// Created: MCO 2011-07-05
// =============================================================================
class ADN_WeaponFilter : public ADN_Connector_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ADN_WeaponFilter();
    //@}

    //! @name Modifiers
    //@{
    void Initialize( ADN_Launchers_Data::LauncherInfos& launcher, bool direct );
    void Initialize( ADN_Resources_Data::AmmoCategoryInfo& ammo, bool direct );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Initialize( ADN_Connector_ABC& connector ) const;
    virtual void SetDataPrivate( void* pData );
    //@}

    //! @name Helpers
    //@{
    bool Check() const;
    //@}

private:
    //! @name Member data
    //@{
    bool bDirect_;
    ADN_Type_Bool* pData_;
    ADN_Launchers_Data::LauncherInfos* pLauncher_;
    ADN_Resources_Data::AmmoCategoryInfo* pAmmo_;
    mutable ADN_Connector_ABC* pConnector_;
    //@}
};

#endif // ADN_WeaponFilter_h
