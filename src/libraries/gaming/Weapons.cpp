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
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
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

void Weapons::DrawEfficientRange( const geometry::Point2f& where, gui::GLView_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_STIPPLE );
    glLineStipple( 1, 0x0F0F );
    glLineWidth( 2 );
    glColor4f( COLOR_WHITE );
    tools.DrawCircle( where, float( efficientRange_ ) );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Weapons::OptionChanged
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void Weapons::OptionChanged( const std::string& name, const kernel::OptionVariant& /*value*/ )
{
    if( name == "EfficientRange/Volume"
        || name == "EfficientRange/Ph"
        || name == "EfficientRange/FilterIndirectWeapon"
        || name == "EfficientRange/IndirectWeapon" )
        UpdateRange();
}

// -----------------------------------------------------------------------------
// Name: Weapons::DoUpdate
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::DoUpdate( const sword::UnitAttributes& message )
{
    if( !message.has_equipment_dotations() )
        return;
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
    minRange_ = std::numeric_limits< unsigned int >::max();
    maxRange_ = 0;
    efficientRange_ = 0;
    const auto filter = controllers_.options_.GetOption( "EfficientRange/FilterIndirectWeapon" ).To< bool >()
        ? weapons_.Find( controllers_.options_.GetOption( "EfficientRange/IndirectWeapon" ).To< QString >().toStdString() )
        : 0;
    tools::Iterator< const Equipment& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        if( equipment.available_ > 0 )
            AddEquipmentRange( equipment.type_, filter );
    }
    if( minRange_ == std::numeric_limits< unsigned int >::max() )
        minRange_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Weapons::AddEquipmentRange
// Created: SBO 2008-08-06
// -----------------------------------------------------------------------------
void Weapons::AddEquipmentRange( const kernel::EquipmentType& type, const kernel::WeaponSystemType* filter )
{
    auto it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::WeaponSystemType& weapon = it.NextElement();
        if( !filter || weapon.GetId() == filter->GetId() )
        {
            minRange_ = std::min( minRange_, weapon.GetMinRange() );
            maxRange_ = std::max( maxRange_, weapon.GetMaxRange() );
        }
        const int ph = controllers_.options_.GetOption( "EfficientRange/Ph" ).To< int >();
        const int volume = controllers_.options_.GetOption( "EfficientRange/Volume" ).To< int >();
        efficientRange_ = std::max( efficientRange_, weapon.GetEfficientRange( volume, 0.01 * ph ) );
    }
}

unsigned int Weapons::GetMinRange() const
{
    return minRange_;
}

unsigned int Weapons::GetMaxRange() const
{
    return maxRange_;
}
