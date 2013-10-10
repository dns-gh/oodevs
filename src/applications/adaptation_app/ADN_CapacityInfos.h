// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_CapacityInfos_h_
#define __ADN_CapacityInfos_h_

#include "ADN_Types.h"

class ADN_ConsistencyChecker;

namespace helpers
{

struct NullType {};

class ADN_TypeCapacity_Infos : public ADN_Ref_ABC
{
public:
    ADN_TypeCapacity_Infos() : bPresent_ ( false ) {}
    virtual ~ADN_TypeCapacity_Infos() {}

    virtual void FixConsistency()
    {
        // NOTHING
    }
    virtual void ReadArchive( xml::xistream& )
    {
        bPresent_ = true;
    }
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& /*checker*/, const ADN_Type_String& /*objectName*/ ) const
    {
        // NOTHING
    }
    virtual void WriteArchive( xml::xostream& ) const
    {
        // NOTHING
    }
public:
    ADN_Type_Bool   bPresent_;
};

enum E_Capacities
{
    eActivableCapacity = 0,
    eAltitudeModifierCapacity,
    eAttitudeModifierCapacity,
    eAttritionCapacity,
    eAvoidableCapacity,
    eBridgingCapacity,
    eBuildableCapacity,
    eBurnCapacity,
    eBypassableCapacity,
    eConstructorCapacity,
    eContaminationCapacity,
    eDecontaminationCapacity,
    eDelayCapacity,
    eDetectionCapacity,
    eFirePropagationModifierCapacity,
    eFloodCapacity,
    eImprovableCapacity,
    eInteractionHeightCapacity,
    eInteractWithSideCapacity,
    eInterferenceCapacity,
    eIntoxicationCapacity,
    eLodgingCapacity,
    eLogisticCapacity,
    eMobilityCapacity,
    ePerceptionCapacity,
    ePopulationCapacity,
    ePropagationCapacity,
    eProtectionCapacity,
    eResourceNetworkCapacity,
    eScatteringCapacity,
    eSpawnCapacity,
    eStockCapacity,
    eStructuralCapacity,
    eSupplyCapacity,
    eSupplyRouteCapacity,
    eTerrainHeuristicCapacity,
    eTimeLimitedCapacity,
    eTrafficabilityCapacity,
    eUndergroundNetworkCapacity,
    eUniversalCapacity,
    eUrbanDestructionCapacity,
    eWorkableCapacity,
    eFireForbiddenCapacity,
    eBorderCapacity,
    eDisasterCapacity
};

template< E_Capacities T, typename DefaultFieldsHolderType = NullType >
class ADN_CapacityInfos_Default : public ADN_TypeCapacity_Infos
{
public:
    static const std::string TAG;

    ADN_CapacityInfos_Default() {}
};

}

#endif // __ADN_CapacityInfos_h_
