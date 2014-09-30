// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LayersHelpers_h_
#define __LayersHelpers_h_

#include "ENT/ENT_Enums.h"
#include <boost/optional.hpp>

namespace gui
{
    class Layer_ABC;

    typedef std::shared_ptr< Layer_ABC >            T_Layer;
    typedef std::function< void( const T_Layer& ) > T_LayerFunctor;
    typedef std::map< E_LayerTypes, T_Layer >       T_LayersMap;
    typedef std::vector< T_Layer >                  T_LayersVector;
    typedef std::vector< E_LayerTypes >             T_LayerTypesVector;

    namespace layers
    {
        struct Descriptor
        {
            E_LayerTypes type_;
            std::string passes_;
            bool pickable_;
            bool configurable_;
            int allowedModes_;
            int readOnlyModes_;
            int hiddenModes_;
            std::vector< E_LayerTypes > children_;
        };

        void CheckConsistency();
        const Descriptor& GetDescriptor( E_LayerTypes type );
        T_LayersVector GetDefaultDrawingOrder( const T_LayersMap& map, int mode );
        T_LayersVector GetEventOrder( const T_LayersMap& map, int mode );
        T_LayerTypesVector GetDefaultConfigurableOrder( const T_LayersMap& map, int mode );

    } //! namespace config
} //! namespace gui
#endif // __LayersHelpers_h_
