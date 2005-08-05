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

namespace TEST {

    class Position;
    class Path;
    class Direction;

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
    //! @name Operations
    //@{
    static void CopyId         ( uint              nIdValue  , uint&             asn );
    static void CopyIdList     ( const T_IdVector& ids       , ASN1T_ListOID&    asn );
    static void CopyEnumeration( uint              nEnumValue, uint&             asn );
    static void CopyBool       ( bool              bValue    , ASN1BOOL&         asn );
    static void CopyPosition   ( const Position&   position  , ASN1T_CoordUTM&   asn );
    static void CopyPosition   ( const Position&   position  , ASN1T_Point&      asn );
    static void CopyPath       ( const Path&       path      , ASN1T_Itineraire& asn );
    static void CopyDirection  ( uint              nDirection, ASN1T_Direction&  asn );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             ASN_Tools();
    virtual ~ASN_Tools();
    //@}

};

} // end namespace TEST

#include "ASN_Tools.inl"

#endif // __ASN_Tools_h_
