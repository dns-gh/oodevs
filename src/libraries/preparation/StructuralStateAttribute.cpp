// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "StructuralStateAttribute.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
    : structuralState_( 100 )
    , controllers_    ( controllers )
    , object_         ( object )
    , dictionary_     ( dictionary )
{
    assert( controllers_.modes_ );
    controllers_.modes_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute destructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::~StructuralStateAttribute()
{
    assert( controllers_.modes_ );
    controllers_.modes_->Unregister( *this );
}

namespace
{
    struct StructuralSetter
    {
        void operator()( unsigned int* pValue, unsigned int value )
        {
            *pValue = std::min( 100u, std::max( 0u, value ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SerializeAttributes
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    if( GetCurrentMode() == ePreparationMode_Exercise )
        xos << xml::start( "structural-state" )
                << xml::attribute( "value", structuralState_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::IsOverriden
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
bool StructuralStateAttribute::IsOverriden() const
{
    return structuralState_ != 100;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::Update
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void StructuralStateAttribute::Update( xml::xistream& xis )
{
    xis >> xml::attribute( "value", structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::NotifyModeChanged
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void StructuralStateAttribute::NotifyModeChanged( int newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    if( newMode == ePreparationMode_Exercise )
        dictionary_.Register( object_, tools::translate( "StructuralStateAttribute", "Info/StructuralState" ), structuralState_, StructuralSetter() );
    else if( newMode == ePreparationMode_Terrain )
        dictionary_.Remove( tools::translate( "StructuralStateAttribute", "Info/StructuralState" ) );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::GetValue
// Created: LGY 2012-06-15
// -----------------------------------------------------------------------------
unsigned int StructuralStateAttribute::GetValue() const
{
    return structuralState_;
}
