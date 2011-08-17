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

namespace helpers
{

struct NullType {};

class ADN_TypeCapacity_Infos
    : public ADN_Ref_ABC
    , public ADN_DataTreeNode_ABC
{
public:
    ADN_TypeCapacity_Infos() : bPresent_ ( false ) {}
    virtual ~ADN_TypeCapacity_Infos() {}

    virtual void ReadArchive( xml::xistream& )
    {
        bPresent_ = true;
    }
    virtual void WriteArchive( xml::xostream& ) {}
public:
    ADN_Type_Bool   bPresent_;
};

enum E_Capacities
{
    eActivableCapacity = 0,
    eAttritionCapacity,
    eAvoidableCapacity,
    eBridgingCapacity,
    eBuildableCapacity,
    eBypassableCapacity,
    eConstructorCapacity,
    eContaminationCapacity,
    eDecontaminationCapacity,
    eDetectionCapacity,
    eExtinguishableCapacity,
    eImprovableCapacity,
    eInteractionHeightCapacity,
    eInteractWithEnemyCapacity,
    eInterferenceCapacity,
    eIntoxicationCapacity,
    eLogisticCapacity,
    eMedicalCapacity,
    eMobilityCapacity,
    eOccupableCapacity,
    ePropagationCapacity,
    eProtectionCapacity,
    eSupplyCapacity,
    eSupplyRouteCapacity,
    eTerrainHeuristicCapacity,
    eTimeLimitedCapacity,
    eWorkableCapacity,
    eSpawnCapacity,
    eStructuralCapacity,
    eAttitudeModifierCapacity,
    ePerceptionCapacity,
    eScatteringCapacity,
    eDelayCapacity,
    eFirePropagationModifierCapacity,
    eFloodCapacity,
    eBurnCapacity,
    eUniversalCapacity,
    eResourceNetworkCapacity,
    eLodgingCapacity,
    eAltitudeModifierCapacity,
    eStockCapacity,
    eUndergroundNetworkCapacity,
    eSpawnObjectCapacity
};

template< E_Capacities T, typename DefaultFieldsHolderType = NullType >
class ADN_CapacityInfos_Default
    : public ADN_TypeCapacity_Infos
{
public:
    static const std::string DISPLAY_NAME;
    static const std::string TAG;

    ADN_CapacityInfos_Default() {}

    virtual void ReadArchive( xml::xistream& xis )
    {
        ADN_TypeCapacity_Infos::ReadArchive( xis );
    }
    virtual void WriteArchive( xml::xostream& /*xos*/ ) {}
};

}

#endif // __ADN_CapacityInfos_h_
