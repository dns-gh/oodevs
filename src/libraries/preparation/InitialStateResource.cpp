// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InitialStateResource.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: InitialStateResource::InitialStateResource
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateResource::InitialStateResource( const QString& name, const QString& category, unsigned int number, unsigned int maximum,
                                            double lowThreshold, double highThreshold, double consumption )
    : name_         ( name )
    , category_     ( category )
    , number_       ( number )
    , maximum_      ( maximum )
    , lowThreshold_ ( lowThreshold )
    , highThreshold_( highThreshold )
    , consumption_  ( consumption )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateResource::InitialStateResource
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateResource::InitialStateResource( xml::xistream& xis )
    : maximum_      ( 0 )
    , number_       ( 0 )
    , consumption_  ( 0 )
    , lowThreshold_ ( -1 )
    , highThreshold_( -1 )
{
    std::string name;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "quantity", number_ )
        >> xml::optional >> xml::attribute( "low-threshold", lowThreshold_ )
        >> xml::optional >> xml::attribute( "high-threshold", highThreshold_ );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: InitialStateResource::~InitialStateResource
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateResource::~InitialStateResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateResource::Serialize
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
void InitialStateResource::Serialize( xml::xostream& xos, double defaultLowThreshold, double defaultHighThreshold ) const
{
    xos << xml::start( "resource" )
        << xml::attribute( "name", name_ )
        << xml::attribute( "quantity", number_ );
    if( lowThreshold_ != defaultLowThreshold )
        xos.attribute( "low-threshold", lowThreshold_ );
    if( highThreshold_ != defaultHighThreshold )
        xos.attribute( "high-threshold", highThreshold_ );
    xos << xml::end; // resource
}

// -----------------------------------------------------------------------------
// Name: InitialStateResource::operator==
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateResource::operator==( const InitialStateResource& object ) const
{
    return name_ == object.name_ && category_ == object.category_ && number_ == object.number_ && lowThreshold_ == object.lowThreshold_ && highThreshold_ == object.highThreshold_ && consumption_ == object.consumption_;
}

// -----------------------------------------------------------------------------
// Name: InitialStateResource::operator!=
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateResource::operator!=( const InitialStateResource& object ) const
{
    return !( *this == object );
}
