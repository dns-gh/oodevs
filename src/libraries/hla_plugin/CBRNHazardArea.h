// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_CBRN_Hazard_Area_H_
#define plugins_hla_CBRN_Hazard_Area_H_

#include "rpr/Coordinates.h"
#include "SerializationTools.h"
#include <vector>

namespace plugins
{
namespace hla
{

enum AgentClassEnum8
{
    agentClassEnum8_Chemical = 0,
    agentClassEnum8_Biological = 1,
    agentClassEnum8_Radiological = 2,
    agentClassEnum8_Nuclear = 3
};


enum ATP45HazardAreaTypeEnum8
{
    aTP45HazardAreaTypeEnum8_Simple = 0,
    aTP45HazardAreaTypeEnum8_Detailed = 1
};

enum AgentTypeEnum16
{
    agentTypeEnum16_Chemical = 1000,
    agentTypeEnum16_Chocking = 1100,
    agentTypeEnum16_CGl = 1101,
    agentTypeEnum16_Persistent_Nerve = 1200
};

enum ContourHazardTypeEnum8
{
    contourHazardTypeEnum8_Concentration = 0,
    contourHazardTypeEnum8_Deposition = 1,
    contourHazardTypeEnum8_Dosage = 2,
    contourHazardTypeEnum8_Radioligical_Dose = 3,
    contourHazardTypeEnum8_Radioligical_Dose_Rate = 4,
};

struct RawDataHazardContour
{
    RawDataHazardContour() : exposureLevel(0.f){}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        static const uint32_t sz = static_cast<uint32_t>(locations.size());
        archive << exposureLevel;
        if( locations.empty() )
            return;
        archive << sz;
        archive << locations;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32_t sz = 0;
        archive >> exposureLevel;
        if( archive.GetSize() == 0 )
            return;
        locations.clear();
        archive >> sz;
        locations.resize( sz );
        archive >> locations;
    }
    //@}

    float exposureLevel;
    std::vector< rpr::WorldLocation > locations;
};

} // namespace hla
} // namespace plugins

#endif // plugins_hla_CBRN_Hazard_Area_H_
