// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Lima_h_
#define __MIL_Lima_h_

#include "MIL.h"

#include "MIL_TacticalLine_ABC.h"

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class MIL_Lima : public MIL_TacticalLine_ABC
{
    MT_COPYNOTALLOWED( MIL_Lima )

public:
    MIL_Lima();
    MIL_Lima( const ASN1T_MsgLimaCreationRequest& asn );
    MIL_Lima( const MIL_Automate& automateBroadcasted  , MIL_InputArchive& archive );
    MIL_Lima( const MIL_Formation& formationBroadcasted, MIL_InputArchive& archive );
    virtual ~MIL_Lima();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Update       ( const ASN1T_MsgLimaUpdateRequest& asn ); 
    void SendFullState() const;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void SendMsg() const;
    //@}
};

#include "MIL_Lima.inl"

#endif // __MIL_Lima_h_
