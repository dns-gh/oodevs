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
    static void CopyID                 ( unsigned int                     nIdValue  , OID&                  asn );
    static void CopyIDList             ( const T_IdVector&        ids       , ListOID&              asn );


    static void CopyAgent              ( unsigned int                     nIdValue      , Unit&                asn );
    static void CopyAgentList          ( const T_IdVector&        ids           , UnitList&            asn );
    static void CopyAgentKnowledge     ( unsigned int                     nIdAgent      , MsgUnitKnowledge& asn );
    static void CopyAgentKnowledgeList ( const T_IdVector&        ids           , UnitKnowledgeList&   asn );
    static void CopyAutomate           ( unsigned int                     nIdValue      , Automat&             asn );
    static void CopyAutomateList       ( const T_IdVector&        ids           , AutomatList&         asn );
    static void CopyObjectKnowledge    ( unsigned int                     nIdObject     , MsgObjectKnowledge& asn );
    static void CopyObjectKnowledgeList( const T_IdVector&        ids           , ObjectKnowledgeList&  asn );
    static void CopyGenObject          ( const GenObject&         genObject     , MsgPlannedWork&     asn );
    static void CopyGenObjectList      ( const T_GenObjectVector& genObjects    , PlannedWorkList& asn );
    static void CopyBool               ( bool                     bValue        , ASN1BOOL&                   asn );
    static void CopyNumeric            ( int                      nValue        , ASN1INT&                    asn );
    static void CopyNumeric            ( int                      nValue        , ASN1REAL&                   asn );
    static void CopyPoint              ( const Position&          position      , CoordUTM&             asn );
    static void CopyPoint              ( const Position&          position      , Point&                asn );
    static void CopyPointList          ( const T_PositionVector&  positions     , PointList&            asn );
    static void CopyPath               ( const Path&              path          , Path&           asn );
    static void CopyPathList           ( const T_PathVector&      path          , PathList&       asn );
    static void CopyDirection          ( unsigned int                     nDirection    , Heading&            asn );
    static void CopyLocation           ( const MsgLocation&          location      , Location&         asn );
    static void CopyLocationList       ( const T_LocationVector&  locations     , MsgLocationList&     asn );
    static void CopyPolygon            ( const MsgLocation&          location      , Polygon&              asn );
    static void CopyPolygonList        ( const T_LocationVector&  locations     , PolygonList&          asn );
    static void CopyNatureAtlas        ( unsigned int                     nValue        , AtlasNature&          asn );
    static void CopyGDH                ( unsigned int                     nTime         , DateTime&                  asn );
    static void CopyPopulationKnowledge( unsigned int                     nIdPopulation , PopulationKnowledge&  asn );
    static void CopyMedicalPriorities  ( const T_IdVector&        ids           , LogMedicalPriorities&       asn );
    static void CopyDotationDType      ( unsigned int                     nIdValue      , DotationType&         asn );
    //@}

    template< typename T >
    static void CopyEnumeration( unsigned int nEnumValue, T& asn )
    {
        asn = ( T )nEnumValue;
    }
    //@}

    //! @name Delete Operations
    //@{
    static void Delete( UnitList&            asn );
    static void Delete( AutomatList&         asn );
    static void Delete( Polygon&              asn );
    static void Delete( PolygonList&          asn );
    static void Delete( PointList&            asn );
    static void Delete( PathList&       asn );
    static void Delete( MsgLocationList&     asn );
    static void Delete( UnitKnowledgeList&   asn );
    static void Delete( ObjectKnowledgeList&  asn );
    static void Delete( MsgPlannedWork&     asn );
    static void Delete( PlannedWorkList& asn );
    static void Delete( LogMedicalPriorities&       asn );
    //@}

    //! @name Enumeration translation to/from strings
    //@{
    static const char* ToString( const EnumOrderErrorCode& nCode );
    static const char* ToString( const EnumSetAutomatModeErrorCode& nCode );
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
