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

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Messages/ASN_Types.h"
#include "Tools/Position.h"
#include "Tools/Path.h"
#include "Tools/Location.h"

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
    static void CopyID                 ( uint                    nIdValue  , ASN1T_OID&                  asn );
    static void CopyIDList             ( const T_IdVector&       ids       , ASN1T_ListOID&              asn );

    static void CopyAgent              ( uint                    nIdValue  , ASN1T_Agent&                asn );
    static void CopyAgentList          ( const T_IdVector&       ids       , ASN1T_ListAgent&            asn );
    static void CopyAgentKnowledge     ( uint                    nIdAgent  , ASN1T_KnowledgeAgent&       asn );
    static void CopyAgentKnowledgeList ( const T_IdVector&       ids       , ASN1T_ListKnowledgeAgent&   asn );
    static void CopyAutomate           ( uint                    nIdValue  , ASN1T_Automate&             asn );
    static void CopyAutomateList       ( const T_IdVector&       ids       , ASN1T_ListAutomate&         asn );
    static void CopyObjectKnowledge    ( uint                    nIdObject , ASN1T_KnowledgeObject&      asn );
    static void CopyObjectKnowledgeList( const T_IdVector&       ids       , ASN1T_ListKnowledgeObject&  asn );
    static void CopyGenObject          ( uint                    nIdValue  , ASN1T_MissionGenObject&     asn );
    static void CopyGenObjectList      ( const T_IdVector&       ids       , ASN1T_ListMissionGenObject& asn );
    static void CopyBool               ( bool                    bValue    , ASN1BOOL&                   asn );
    static void CopyNumeric            ( int                     nValue    , ASN1INT&                    asn );
    static void CopyNumeric            ( int                     nValue    , ASN1REAL&                   asn );
    static void CopyPoint              ( const Position&         position  , ASN1T_CoordUTM&             asn );
    static void CopyPoint              ( const Position&         position  , ASN1T_Point&                asn );
    static void CopyPointList          ( const T_PositionVector& positions , ASN1T_ListPoint&            asn );
    static void CopyPath               ( const Path&             path      , ASN1T_Itineraire&           asn );
    static void CopyPathList           ( const T_PathVector&     path      , ASN1T_ListItineraire&       asn );
    static void CopyDirection          ( uint                    nDirection, ASN1T_Direction&            asn );
    static void CopyLocation           ( const Location&         location  , ASN1T_Localisation&         asn );
    static void CopyPolygon            ( const Location&         location  , ASN1T_Polygon&              asn );
    static void CopyPolygonList        ( const T_LocationVector& locations , ASN1T_ListPolygon&          asn );
    static void CopyNatureAtlas        ( uint                    nValue    , ASN1T_NatureAtlas&          asn );
    static void CopyGDH                ( uint                    nTime     , ASN1T_GDH&                  asn );

    template< typename T >
    static void CopyEnumeration( uint nEnumValue, T& asn )
    {
        asn = ( T )nEnumValue;
    }
    //@}

    //! @name Delete Operations
    //@{
    static void Delete( ASN1T_ListAgent&            asn );
    static void Delete( ASN1T_ListAutomate&         asn );
    static void Delete( ASN1T_Polygon&              asn );
    static void Delete( ASN1T_ListPolygon&          asn );
    static void Delete( ASN1T_ListPoint&            asn );
    static void Delete( ASN1T_ListItineraire&       asn );
    static void Delete( ASN1T_ListLocalisation&     asn );
    static void Delete( ASN1T_ListKnowledgeAgent&   asn );
    static void Delete( ASN1T_ListKnowledgeObject&  asn );
    static void Delete( ASN1T_ListMissionGenObject& asn );
    //@}

    //! @name Enumeration translation to/from strings
    //@{
    static const char* ToString( const ASN1T_EnumOrderErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumSetAutomateModeErrorCode& nCode );
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
