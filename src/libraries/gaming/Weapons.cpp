// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Weapons.h"
#include "Equipment.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/WeaponSystemType.h"

// -----------------------------------------------------------------------------
// Name: Weapons constructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
Weapons::Weapons( const kernel::Resolver_ABC< kernel::EquipmentType >& equipments, const kernel::Resolver_ABC< kernel::WeaponSystemType, std::string >& weapons )
    : equipments_( equipments )
    , weapons_( weapons )
    , minRange_( 0 )
    , maxRange_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weapons destructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
Weapons::~Weapons()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weapons::Draw
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if(  tools.ShouldDisplay( "WeaponRanges" ) && viewport.IsVisible( where ) 
      && ( minRange_ > 0 || maxRange_ > 0 ) )
    {
        glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT );
            glEnable( GL_LINE_STIPPLE );
            glLineStipple( 1, 0x0F0F );
            glLineWidth( 2 );
            if( minRange_ > 0 )
                tools.DrawCircle( where, float( minRange_ ) );
            if( maxRange_ > 0 )
                tools.DrawCircle( where, float( maxRange_ ) );
            glDisable( GL_LINE_STIPPLE );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: Weapons::DoUpdate
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if ( message.m.dotation_eff_materielPresent != 1 )
        return;
    minRange_ = std::numeric_limits< unsigned int >::max();
    maxRange_ = 0;
    for( unsigned int i = 0; i < message.dotation_eff_materiel.n; ++i )
    {
        const ASN1T_EquipmentDotations& value = message.dotation_eff_materiel.elem[ i ];
        Equipment* equipment = Find( value.type_equipement );
        if( !equipment )
        {
            equipment = new Equipment( equipments_.Get( value.type_equipement ) );
            Register( value.type_equipement, *equipment );
        }
        equipment->Update( value );
    }
    UpdateRange();
}

// -----------------------------------------------------------------------------
// Name: Weapons::UpdateRange
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::UpdateRange()
{
    kernel::Iterator< const Equipment& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        if( equipment.available_ > 0 )
            AddEquipmentRange( equipment.type_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Weapons::AddEquipmentRange
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::AddEquipmentRange( const kernel::EquipmentType& type )
{
    kernel::Iterator< const kernel::WeaponSystemType& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::WeaponSystemType& weapon = it.NextElement();
        minRange_ = std::min( minRange_, weapon.GetMinRange() );
        maxRange_ = std::max( maxRange_, weapon.GetMaxRange() );
    }
}
