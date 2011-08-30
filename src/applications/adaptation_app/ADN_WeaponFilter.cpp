// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_WeaponFilter.h"
#include "moc_ADN_WeaponFilter.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter constructor
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
ADN_WeaponFilter::ADN_WeaponFilter()
    : pData_     ( 0 )
    , pConnector_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Initialize
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::Initialize( ADN_Type_Bool* pData, T_Validate validate )
{
    validate_ = validate;
    pData_ = pData;
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Initialize
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::Initialize( ADN_Connector_ABC& connector ) const
{
    if( pData_ )
    {
        pData_->Initialize( connector );
        pConnector_ = &connector;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::SetDataPrivate
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::SetDataPrivate( void* pData )
{
    const bool data = *(bool*)pData;
    if( pData_ )
    {
        if( data || Check() )
            *pData_ = data;
        pData_->Initialize( *pConnector_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Check
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
bool ADN_WeaponFilter::Check() const
{
    ADN_Weapons_Data::T_WeaponInfosVector weapons = ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos();
    QString invalidWeapons;
    for( ADN_Weapons_Data::T_WeaponInfosVector::iterator it = weapons.begin(); it != weapons.end(); ++it )
        if( validate_( **it ) )
            invalidWeapons += ((*it)->strName_.GetData() + '\n').c_str();
    if( invalidWeapons.isEmpty() )
        return true;
    int result = QMessageBox::warning( 0,
                                       tr( "Warning" ),
                                       tr( "This will delete all the following associated weapon systems :" ) + "\n\n" + invalidWeapons + '\n' + tr( "Proceed anyway ?" ),
                                       QMessageBox::Ok, QMessageBox::Cancel );
    if( result == QMessageBox::Cancel )
        return false;
    for( ADN_Weapons_Data::T_WeaponInfosVector::iterator it = weapons.begin(); it != weapons.end(); ++it )
        if( validate_( **it ) )
            ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos().RemItem( *it );
    return true;
}
