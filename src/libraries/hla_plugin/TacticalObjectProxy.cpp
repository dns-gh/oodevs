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
#include "SimulationTimeManager_ABC.h"
#include "dispatcher/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "dispatcher/Localisation.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "dispatcher/PropagationAttribute.h"
#include "dispatcher/Object_ABC.h"
#include "dispatcher/ObjectAttributeContainer.h"
#include <tools/Path.h>
#include <tools/FileWrapper.h>
#include "propagation/PropagationManager.h"
#include "propagation/ASCExtractor.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

#include <set>

using namespace plugins::hla;

namespace
{
    void readResource( xml::xistream& xis, const rpr::EntityTypeResolver_ABC& dotationResolver, std::vector< rpr::EntityType >& resources )
    {
        const std::string name( xis.attribute< std::string >( "name" ) );
        rpr::EntityType val;
        dotationResolver.Find( name, val );
        resources.push_back( val );
    }
   const dispatcher::PropagationAttribute* getPropagationAttribute(const dispatcher::Object_ABC& object)
   {
       const dispatcher::PropagationAttribute* retval = 0;
       for( auto it = object.GetAttributes().GetAttributes().begin(); it!=object.GetAttributes().GetAttributes().end(); ++it)
       {
           retval = dynamic_cast< const dispatcher::PropagationAttribute* >( &*it );
           if( retval )
               break;
       }
       return retval;
   }
   void readPropagation( const dispatcher::PropagationAttribute& attribute, ObjectPropagationEventListener_ABC::T_DataVector& data, const SimulationTimeManager_ABC& timeManager )
   {
       const tools::Path path = tools::Path::FromUTF8( attribute.GetModel() );
       ::PropagationManager propMgr;
       propMgr.Initialize( path, attribute.GetDate() );
       const tools::Path::T_Paths files = propMgr.GetFiles( !attribute.GetDate().empty() ? attribute.GetDate() : timeManager.getSimulationTime() );
       if( files.empty() )
           return;
       std::for_each( files.begin(), files.end(), [&]( const tools::Path& f ) {
           const ASCExtractor extractor( f, propMgr.GetProjectionFile() );
           ObjectPropagationEventListener_ABC::Data values( extractor.GetCols(), extractor.GetRows(), extractor.GetExtent(), extractor.GetMaximumValue() );
           values.concentrations = extractor.GetValues();
           data.push_back( values );
       });
   }
}


// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy constructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectProxy::TacticalObjectProxy( dispatcher::Object_ABC& object, const rpr::EntityTypeResolver_ABC& dotationResolver, const SimulationTimeManager_ABC& timeManager )
    : dispatcher::Observer< sword::ObjectUpdate >( object )
    , object_( object )
    , dotationResolver_( dotationResolver )
    , timeManager_( timeManager )
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
    const dispatcher::PropagationAttribute* prop = getPropagationAttribute( object_ );
    if( prop )
    {
        ObjectPropagationEventListener_ABC::T_DataVector data;
        readPropagation( *prop, data, timeManager_ );
        Apply( listener, &ObjectPropagationEventListener_ABC::PropagationChanged, data );
    }
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

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Unregister
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Notify( const sword::ObjectUpdate&  )
{
    const dispatcher::PropagationAttribute* prop = getPropagationAttribute( object_ );
    if( prop )
    {
        ObjectPropagationEventListener_ABC::T_DataVector data;
        readPropagation( *prop, data, timeManager_ );
        listeners_.Apply( &ObjectPropagationEventListener_ABC::PropagationChanged, data );
    }
}
