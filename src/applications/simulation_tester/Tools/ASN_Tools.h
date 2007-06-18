// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __ASN_Tools_h_
#define __ASN_Tools_h_

#include "Types.h"
#include "Messages/ASN_Types.h"
#include "Tools/Position.h"
#include "Tools/Path.h"
#include "Tools/Location.h"
#include "Entities/GenObject.h"

namespace TEST {


// =============================================================================
/** @class  ASN_Tools
    @brief  ASN_Tools
    @par    Using example
    @code
    ASN_Tools;
    @endcode
*/
// Created: SBO 2005-08-05
// =============================================================================
class ASN_Tools
{

public:
    //! @name Copy Operations
    //@{
    static void CopyID                 ( uint                     nIdValue  , ASN1T_OID&                  asn );
    static void CopyIDList             ( const T_IdVector&        ids       , ASN1T_ListOID&              asn );


    static void CopyAgent              ( uint                     nIdValue      , ASN1T_Unit&                asn );
    static void CopyAgentList          ( const T_IdVector&        ids           , ASN1T_UnitList&            asn );
    static void CopyAgentKnowledge     ( uint                     nIdAgent      , ASN1T_UnitKnowledge&       asn );
    static void CopyAgentKnowledgeList ( const T_IdVector&        ids           , ASN1T_UnitKnowledgeList&   asn );
    static void CopyAutomate           ( uint                     nIdValue      , ASN1T_Automat&             asn );
    static void CopyAutomateList       ( const T_IdVector&        ids           , ASN1T_AutomatList&         asn );
    static void CopyObjectKnowledge    ( uint                     nIdObject     , ASN1T_ObjectKnowledge&      asn );
    static void CopyObjectKnowledgeList( const T_IdVector&        ids           , ASN1T_ObjectKnowledgeList&  asn );
    static void CopyGenObject          ( const GenObject&         genObject     , ASN1T_PlannedWork&     asn );
    static void CopyGenObjectList      ( const T_GenObjectVector& genObjects    , ASN1T_PlannedWorkList& asn );
    static void CopyBool               ( bool                     bValue        , ASN1BOOL&                   asn );
    static void CopyNumeric            ( int                      nValue        , ASN1INT&                    asn );
    static void CopyNumeric            ( int                      nValue        , ASN1REAL&                   asn );
    static void CopyPoint              ( const Position&          position      , ASN1T_CoordUTM&             asn );
    static void CopyPoint              ( const Position&          position      , ASN1T_Point&                asn );
    static void CopyPointList          ( const T_PositionVector&  positions     , ASN1T_PointList&            asn );
    static void CopyPath               ( const Path&              path          , ASN1T_Path&           asn );
    static void CopyPathList           ( const T_PathVector&      path          , ASN1T_PathList&       asn );
    static void CopyDirection          ( uint                     nDirection    , ASN1T_Heading&            asn );
    static void CopyLocation           ( const Location&          location      , ASN1T_Location&         asn );
    static void CopyLocationList       ( const T_LocationVector&  locations     , ASN1T_LocationList&     asn );
    static void CopyPolygon            ( const Location&          location      , ASN1T_Polygon&              asn );
    static void CopyPolygonList        ( const T_LocationVector&  locations     , ASN1T_PolygonList&          asn );
    static void CopyNatureAtlas        ( uint                     nValue        , ASN1T_AtlasNature&          asn );
    static void CopyGDH                ( uint                     nTime         , ASN1T_DateTime&                  asn );
    static void CopyPopulationKnowledge( uint                     nIdPopulation , ASN1T_PopulationKnowledge&  asn );
    static void CopyMedicalPriorities  ( const T_IdVector&        ids           , ASN1T_LogMedicalPriorities&       asn );
    static void CopyDotationDType      ( uint                     nIdValue      , ASN1T_DotationType&         asn ); 
    //@}

    template< typename T >
    static void CopyEnumeration( uint nEnumValue, T& asn )
    {
        asn = ( T )nEnumValue;
    }
    //@}

    //! @name Delete Operations
    //@{
    static void Delete( ASN1T_UnitList&            asn );
    static void Delete( ASN1T_AutomatList&         asn );
    static void Delete( ASN1T_Polygon&              asn );
    static void Delete( ASN1T_PolygonList&          asn );
    static void Delete( ASN1T_PointList&            asn );
    static void Delete( ASN1T_PathList&       asn );
    static void Delete( ASN1T_LocationList&     asn );
    static void Delete( ASN1T_UnitKnowledgeList&   asn );
    static void Delete( ASN1T_ObjectKnowledgeList&  asn );
    static void Delete( ASN1T_PlannedWork&     asn );
    static void Delete( ASN1T_PlannedWorkList& asn );
    static void Delete( ASN1T_LogMedicalPriorities&       asn );
    //@}

    //! @name Enumeration translation to/from strings
    //@{
    static const char* ToString( const ASN1T_EnumOrderErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumSetAutomatModeErrorCode& nCode );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             ASN_Tools();
    virtual ~ASN_Tools();
    //@}

};

} // end namespace TEST

#endif // __ASN_Tools_h_
