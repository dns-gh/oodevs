// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ASN_Delete_h_
#define __ASN_Delete_h_

#include "Simulation.h"

// =============================================================================
/** @class  ASN_Delete
    @brief  ASN_Delete
*/
// Created: LDC 2009-10-05
// =============================================================================
namespace ASN_Delete
{
    void Delete( ASN1T_MissionObjective&         asn );
    void Delete( ASN1T_MissionObjectiveList&     asn );
    void Delete( ASN1T_PlannedWork&              asn );
    void Delete( ASN1T_PlannedWorkList&          asn );
    void Delete( ASN1T_UnitList&                 asn );
    void Delete( ASN1T_AutomatList&              asn );
    void Delete( ASN1T_Polygon&                  asn );
    void Delete( ASN1T_PolygonList&              asn );
    void Delete( ASN1T_PointList&                asn );
    void Delete( ASN1T_PathList&                 asn );
    void Delete( ASN1T_LocationList&             asn );
    void Delete( ASN1T_UnitKnowledgeList&        asn );
    void Delete( ASN1T_ObjectKnowledgeList&      asn );
    void Delete( ASN1T_LogMedicalPriorities&     asn );
    void Delete( ASN1T_LogMaintenancePriorities& asn );
    void Delete( ASN1T_CoordLatLongList& asn );
}

#endif // __ASN_Delete_h_
