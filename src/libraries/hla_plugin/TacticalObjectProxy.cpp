// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectProxy.h"
#include "TacticalObjectEventListener_ABC.h"
#include "dispatcher/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "dispatcher/Localisation.h"
#include "rpr/EntityTypeResolver_ABC.h"

#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    void readResource( xml::xistream& xis, const rpr::EntityTypeResolver_ABC& dotationResolver, std::vector< rpr::EntityType >& resources )
    {
        const std::string name( xis.attribute< std::string >( "name" ) );
        rpr::EntityType val;
        dotationResolver.Find( name, val );
        resources.push_back( val );
    }
}

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy constructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectProxy::TacticalObjectProxy( dispatcher::Object_ABC& object, const rpr::EntityTypeResolver_ABC& dotationResolver  )
    : object_( object )
    , dotationResolver_( dotationResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy destructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectProxy::~TacticalObjectProxy()
{
    // NOTHING
}

namespace
{
    template< typename T, typename A0 >
    void Apply( TacticalObjectEventListener_ABC& listener, void (T::*func)( const A0&), const A0& v0)
    {
        T* tmp = dynamic_cast< T* >( &listener );
        if( tmp != 0 )
        {
            (tmp->*func)( v0 );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Register
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Register( TacticalObjectEventListener_ABC& listener )
{
    listeners_.Register( listener );
    const dispatcher::Localisation::T_PositionVector& points( object_.GetLocalisation().GetPoints() );
    Apply( listener, &ObjectLocationEventListener_ABC::SpatialChanged, points );
    //listener.SpatialChanged( points );
    kernel::ObjectType::CIT_Capacities it( object_.GetType().CapacitiesBegin() );

    while( object_.GetType().CapacitiesEnd() != it && it->first != "constructor" )
        ++it;
    if( object_.GetType().CapacitiesEnd() != it && it->second.get() != 0 )
    {
        std::vector< rpr::EntityType > resources;
        xml::xistream& xis( *it->second );
        xis >> xml::optional >> xml::start( "buildable" )
                >> xml::optional >> xml::start( "resources" )
                    >> xml::list( "resource", boost::bind( &readResource, _1, boost::cref( dotationResolver_ ), boost::ref( resources ) ) )
                >> xml::end
            >> xml::end
            >> xml::optional >> xml::start( "improvable" )
                >> xml::optional >> xml::start( "resources" )
                    >> xml::list( "resource", boost::bind( &readResource, _1, boost::cref( dotationResolver_ ), boost::ref( resources ) ) )
                >> xml::end
            >> xml::end;
        //listener.ResourcesChanged( resources );
        Apply( listener, &ObjectLocationEventListener_ABC::ResourcesChanged, resources );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Unregister
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Unregister( TacticalObjectEventListener_ABC& listener )
{
    listeners_.Unregister( listener );
}
