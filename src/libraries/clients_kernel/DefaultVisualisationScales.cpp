// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DefaultVisualisationScales.h"

using namespace kernel;

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

const DefaultVisualisationScales DefaultVisualisationScales::data_[] = {
    { eVisualisationScale_LargeTexts,     10000, 100000 },
    { eVisualisationScale_SmallTexts,     0, 25000 },
    { eVisualisationScale_Edges,          0, 100000 },
    { eVisualisationScale_Cliffs,         0, 25000 },
    { eVisualisationScale_Highways,       0, 10000000 },
    { eVisualisationScale_MainRoads,      0, 500000 },
    { eVisualisationScale_SecondaryRoads, 0, 50000 },
    { eVisualisationScale_CountryRoads,   0, 10000 },
    { eVisualisationScale_Bridges,        0, 500000 },
    { eVisualisationScale_Railroads,      0, 50000 },
    { eVisualisationScale_MainRivers,     0, 10000000 },
    { eVisualisationScale_Rivers,         0, 500000 },
    { eVisualisationScale_Streams,        0, 50000 },
    { eVisualisationScale_UrbanBlocks,    0, 25000 }
};
const size_t DefaultVisualisationScales::size_ = COUNT_OF( DefaultVisualisationScales::data_ );
