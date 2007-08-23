// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Limit_h_
#define __MIL_Limit_h_

#include "MIL.h"

#include "MIL_TacticalLine_ABC.h"

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class MIL_Limit : public MIL_TacticalLine_ABC
{
    MT_COPYNOTALLOWED( MIL_Limit )

public:
    MIL_Limit();
    MIL_Limit( const ASN1T_MsgLimitCreationRequest& asn );
    MIL_Limit( const MIL_Automate& automateBroadcasted  , xml::xistream& xis );
    MIL_Limit( const MIL_Formation& formationBroadcasted, xml::xistream& xis );
    virtual ~MIL_Limit();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Update       ( const ASN1T_MsgLimitUpdateRequest& asn ); 
    void SendFullState() const;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void SendMsg() const;
    //@}
};

#include "MIL_Limit.inl"

#endif // __MIL_Limit_h_
