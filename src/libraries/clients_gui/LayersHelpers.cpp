// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LayersHelpers.h"
#include <tools/Exception.h>
#include <boost/assign.hpp>

using namespace gui;

namespace ba = boost::assign;

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

namespace
{

const T_LayerTypesVector defaultDisplayOrder_ = ba::list_of
    // always on bottom layers
    ( eLayerTypes_Default )
    ( eLayerTypes_Elevation3d )
    // configurable layers
    ( eLayerTypes_Elevation2d )
    ( eLayerTypes_Raster )
    ( eLayerTypes_Mapnik )
    ( eLayerTypes_Terrain )
    ( eLayerTypes_ContourLines )
    ( eLayerTypes_Urban )
    ( eLayerTypes_WeatherComposite )
    ( eLayerTypes_VisionCones )
    ( eLayerTypes_ObjectsComposite )
    ( eLayerTypes_CrowdsComposite )
    ( eLayerTypes_InhabitantsComposite )
    ( eLayerTypes_TacticalLinesComposite )
    ( eLayerTypes_UnitsComposite )
    ( eLayerTypes_Grid )
    // always on top layers
    ( eLayerTypes_Actions )
    ( eLayerTypes_Parameters )
    ( eLayerTypes_Metric )
    ( eLayerTypes_Creations )
    ( eLayerTypes_EventCreation )
    ( eLayerTypes_ObjectCreation )
    ( eLayerTypes_IndicatorCreation )
    ( eLayerTypes_InhabitantCreation )
    ( eLayerTypes_ResourceNetworks )
    ( eLayerTypes_Locations )
    ( eLayerTypes_TerrainProfiler )
    ( eLayerTypes_RectangleSelection )
    ( eLayerTypes_Fog );

const T_LayerTypesVector eventOrder_ = ba::list_of< E_LayerTypes >
    ( eLayerTypes_Weather )
    ( eLayerTypes_Elevation3d )
    ( eLayerTypes_Metric )
    ( eLayerTypes_TacticalLinesComposite )
    ( eLayerTypes_RectangleSelection )
    ( eLayerTypes_Urban )
    ( eLayerTypes_ResourceNetworks )
    ( eLayerTypes_ObjectsComposite )
    ( eLayerTypes_InhabitantsComposite )
    ( eLayerTypes_CrowdsComposite )
    ( eLayerTypes_UnitsComposite )
    ( eLayerTypes_Parameters )
    ( eLayerTypes_Terrain );

const layers::Descriptor descriptors_[] = {
    // type                               // passes                           // pick // config
    // allowed modes                      // read only modes                  // hidden modes
    // children
    { eLayerTypes_Actions,                "main",                             false,  false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_AgentKnowledges,        "main",                             true,   false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Agents,                 "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      T_LayerTypesVector() },
    { eLayerTypes_Automats,               "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_ContourLines,           "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Creations,              "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_CrowdKnowledges,        "main",                             true,   false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Crowds,                 "main",                             true,   true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      T_LayerTypesVector() },
    { eLayerTypes_CrowdsComposite,        "main",                             true,   true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      ba::list_of( eLayerTypes_Crowds )( eLayerTypes_CrowdKnowledges ) },
    { eLayerTypes_Default,                "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Drawings,               "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Elevation2d,            "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Elevation3d,            "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_EventCreation,          "main",                             false,  false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Fog,                    "fog",                              false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },                                        
    { eLayerTypes_Formations,             "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },                                        
    { eLayerTypes_Ghosts,                 "main",                             false,  false,
      eModes_AllPrepare,                  eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      T_LayerTypesVector() },
    { eLayerTypes_Grid,                   "main",                             false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_IndicatorCreation,      "main",                             false,  false,
      eModes_AllPrepare,                  eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_InhabitantCreation,     "main",                             false,  false,
      eModes_AllPrepare,                  eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Inhabitants,            "main",                             false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_InhabitantsComposite,   "main",                             true,   true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      ba::list_of( eLayerTypes_Inhabitants )( eLayerTypes_InhabitantCreation ) },
    { eLayerTypes_Locations,              "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Mapnik,                 "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Metric,                 "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_ObjectCreation,         "main",                             false,  false,
      eModes_AllPrepare,                  eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_ObjectKnowledges,       "main",                             true,   false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Objects,                "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      T_LayerTypesVector() },
    { eLayerTypes_ObjectsComposite,       "main",                             true,   true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      ba::list_of( eLayerTypes_Objects )( eLayerTypes_ObjectKnowledges )( eLayerTypes_ObjectCreation ) },
    { eLayerTypes_Parameters,             "main",                             false,  false,
      eModes_All,                         eModes_Itinerary,                   eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Parties,                "main",                             true,   false,
      eModes_AllGaming,                   eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Pathfinds,              "main",                             true,   false,
      eModes_AllGaming,                   eModes_Terrain,                     eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Raster,                 "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_RasterDynamic,          "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_RectangleSelection,              "main",                             false,  false,
      eModes_AllPrepare,                  eModes_Itinerary,                   eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_ResourceNetworks,       "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_TacticalLines,          "main",                             true,   false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_LivingArea,
      T_LayerTypesVector() },
    { eLayerTypes_TacticalLinesComposite, "main",                             true,   true,
      eModes_All,                         eModes_Terrain,                     eModes_None,
      ba::list_of( eLayerTypes_TacticalLines )( eLayerTypes_Drawings )( eLayerTypes_Pathfinds ) },
    { eLayerTypes_Terrain,                "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_TerrainProfiler,        "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Tooltips,               "tooltip",                          false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_UnitsComposite,         "main,tooltip",                     true,   true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      ba::list_of( eLayerTypes_Parties )( eLayerTypes_Formations )( eLayerTypes_Automats )
                 ( eLayerTypes_Ghosts )( eLayerTypes_AgentKnowledges )( eLayerTypes_Agents )
                 ( eLayerTypes_WeaponRanges )( eLayerTypes_Tooltips ) },
    { eLayerTypes_Urban,                  "main",                             true,  true,
      eModes_All,                         eModes_Itinerary,                   eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_VisionCones,            "main",                             false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Watershed,              "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_WeaponRanges,           "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_Weather,                "main",                             false,  false,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      T_LayerTypesVector() },
    { eLayerTypes_WeatherComposite,       "main,composition",                 false,  true,
      eModes_All,                         eModes_Terrain | eModes_Itinerary,  eModes_None,
      ba::list_of( eLayerTypes_Watershed )( eLayerTypes_Weather ) },
};
const size_t descriptorsSize_ = COUNT_OF( descriptors_ );
static_assert( eNbrLayerTypes == descriptorsSize_, "missing layer descriptors" );

} //! anonymous namespace

const layers::Descriptor& layers::GetDescriptor( E_LayerTypes type )
{
    return descriptors_[ type ];
}

void layers::CheckConsistency()
{
    for( int i = 0; i < descriptorsSize_; ++i )
        if( i != descriptors_[ i ].type_ )
            throw MASA_EXCEPTION( QString( "layer descriptor index '%1' does not match it's type '%2'." )
                                  .arg( i ).arg( descriptors_[ i ].type_ ).toStdString() );
}

namespace
{
    T_LayersVector GetAllowedVector( const T_LayersMap& map,
                                     int mode,
                                     const T_LayerTypesVector& order )
    {
        T_LayersVector result;
        for( auto it = order.begin(); it != order.end(); ++it )
        {
            const auto& descriptor = descriptors_[ *it ];
            if( ( descriptor.allowedModes_ & mode ) != 0 &&
                map.count( descriptor.type_ ) )
                result.push_back( map.at( descriptor.type_ ) );
            else if( descriptor.children_.size() > 0 ) // if no composite, seek for children
            {
                T_LayersVector children = GetAllowedVector( map, mode, descriptor.children_ );
                result.insert( result.end(), children.begin(), children.end() );
            }
        }
        return result;
    }
    T_LayerTypesVector GetConfigurableVectorType( const T_LayersMap& map,
                                                  int mode,
                                                  const T_LayerTypesVector& order )
    {
        T_LayerTypesVector result;
        for( auto it = order.begin(); it != order.end(); ++it )
        {
            const auto& descriptor = descriptors_[ *it ];
            if( ( descriptor.allowedModes_ & mode ) != 0 &&
                map.count( descriptor.type_ ) &&
                descriptor.configurable_ )
                result.push_back( descriptor.type_ );
            else if( descriptor.children_.size() > 0 ) // if no composite, seek for children
            {
                T_LayerTypesVector children = GetConfigurableVectorType( map, mode, descriptor.children_ );
                result.insert( result.end(), children.begin(), children.end() );
            }
        }
        return result;
    }
}

T_LayersVector layers::GetDefaultDrawingOrder( const T_LayersMap& map, int mode )
{
    return GetAllowedVector( map, mode, defaultDisplayOrder_ );
}

T_LayersVector layers::GetEventOrder( const T_LayersMap& map, int mode )
{
    return GetAllowedVector( map, mode, eventOrder_ );
}

T_LayerTypesVector layers::GetDefaultConfigurableOrder( const T_LayersMap& map, int mode )
{
    return GetConfigurableVectorType( map, mode, defaultDisplayOrder_ );
}
