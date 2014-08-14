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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Options.h"
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
    , useColor_( false )
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
void Weapons::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "WeaponRanges" ) && viewport.IsVisible( where ) &&
        ( minRange_ > 0 || maxRange_ > 0 || efficientRange_ > 0 ) )
    {
        glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
            // draw the min range disc to the stencil buffer
            glEnable( GL_STENCIL_TEST );
            glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
            glDepthMask( GL_FALSE );
            glStencilFunc( GL_NEVER, 1, 0xFF );
            glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );
            glStencilMask( 0xFF );
            glClear( GL_STENCIL_BUFFER_BIT );
            tools.DrawDisc( where, float( minRange_ ) );
            // draw the max range disc using the stencil buffer to keep the
            // min range disc clear
            glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
            glDepthMask( GL_TRUE );
            glStencilMask( 0x00 );
            glStencilFunc( GL_EQUAL, 0, 0xFF );
            glPushAttrib( GL_CURRENT_BIT );
                static const float alpha = 0.5f;
                if( useColor_ )
                    glColor4f( color_.red() / 255.f, color_.green() / 255.f, color_.blue() / 255.f, alpha );
                else
                {
                    GLfloat color[ 4 ];
                    glGetFloatv( GL_CURRENT_COLOR, color );
                    color[ 3 ] = alpha;
                    glColor4fv( color );
                }
                tools.DrawDisc( where, float( maxRange_ ) );
            glPopAttrib();
            glDisable( GL_STENCIL_TEST );
            glEnable( GL_LINE_STIPPLE );
            glLineStipple( 1, 0x0F0F );
            glLineWidth( 2 );
            tools.DrawCircle( where, float( efficientRange_ ) );
            glDisable( GL_LINE_STIPPLE );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: Weapons::OptionChanged
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
void Weapons::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeVolume"
        || name == "EfficientRangePh"
        || name == "EfficientRangeFilterIndirectWeapon"
        || name == "EfficientRangeIndirectWeapon" )
        UpdateRange();
    else if( name == "EfficientRangeUseCustomColor" )
        useColor_ = value.To< bool >();
    else if( name == "EfficientRangeCustomColor" )
        color_.setNamedColor( value.To< QString >() );
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
    const auto filter = controllers_.options_.GetOption( "EfficientRangeFilterIndirectWeapon", false ).To< bool >()
        ? weapons_.Find( controllers_.options_.GetOption( "EfficientRangeIndirectWeapon", QString() ).To< QString >().toStdString() )
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
        const int ph = controllers_.options_.GetOption( "EfficientRangePh", 50 ).To< int >();
        const int volume = controllers_.options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
        efficientRange_ = std::max( efficientRange_, weapon.GetEfficientRange( volume, 0.01 * ph ) );
    }
}
