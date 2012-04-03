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
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/WeaponSystemType.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Weapons constructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
Weapons::Weapons( kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::EquipmentType >& equipments, const tools::Resolver_ABC< kernel::WeaponSystemType, std::string >& weapons )
    : controllers_( controllers )
    , equipments_( equipments )
    , weapons_( weapons )
    , minRange_( 0 )
    , maxRange_( 0 )
    , efficientRange_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Weapons destructor
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
Weapons::~Weapons()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Weapons::Draw
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "WeaponRanges" ) && viewport.IsVisible( where )
      && ( efficientRange_ > 0 || maxRange_ > 0 || ( minRange_ > 0 && minRange_ < std::numeric_limits< unsigned int >::max() ) ) )
    {
        glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT );
            glEnable( GL_LINE_STIPPLE );
            glLineStipple( 1, 0x0F0F );
            glLineWidth( 2 );
            if( minRange_ > 0 && minRange_ < std::numeric_limits< unsigned int >::max() )
                tools.DrawCircle( where, float( minRange_ ) );
            if( maxRange_ > 0 )
                tools.DrawCircle( where, float( maxRange_ ) );
            if( efficientRange_ > 0 )
                tools.DrawCircle( where, float( efficientRange_ ) );
            glDisable( GL_LINE_STIPPLE );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: Weapons::OptionChanged
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void Weapons::OptionChanged( const std::string& name, const kernel::OptionVariant& )
{
    if( name == "EfficientRangePh" || name == "EfficientRangeVolume" )
    {
        efficientRange_ = 0;
        UpdateRange();
    }
}

// -----------------------------------------------------------------------------
// Name: Weapons::DoUpdate
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_equipment_dotations()  != 1 )
        return;
    minRange_ = std::numeric_limits< unsigned int >::max();
    maxRange_ = 0;
    efficientRange_ = 0;
    for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
    {
        const sword::EquipmentDotations_EquipmentDotation& value = message.equipment_dotations().elem( i );
        Equipment* equipment = Find( value.type().id() );
        if( !equipment )
        {
            equipment = new Equipment( equipments_.Get( value.type().id() ) );
            Register( value.type().id(), *equipment );
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
    tools::Iterator< const Equipment& > it = CreateIterator();
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
    tools::Iterator< const kernel::WeaponSystemType& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::WeaponSystemType& weapon = it.NextElement();
        minRange_ = std::min( minRange_, weapon.GetMinRange() );
        maxRange_ = std::max( maxRange_, weapon.GetMaxRange() );
        int ph = controllers_.options_.GetOption( "EfficientRangePh", 50 ).To< int >();
        int volume = controllers_.options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
        efficientRange_ = std::max( efficientRange_, weapon.GetEfficientRange( volume, 0.01 * ph ) );
    }
}
