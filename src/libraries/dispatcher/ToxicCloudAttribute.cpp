// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ToxicCloudAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::ToxicCloudAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
{
    quantities_.n = 0;
    quantities_.elem = 0;
    Update( asnMsg );    
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::~ToxicCloudAttribute()
{
    Clear();   
}  

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if( asnMsg.m.toxic_cloudPresent )
    {         
        Clear();
        quantities_.n = asnMsg.toxic_cloud.quantities.n;
        quantities_.elem = new ASN1T_LocatedQuantity[ quantities_.n ];
        memcpy( quantities_.elem, asnMsg.toxic_cloud.quantities.elem, sizeof( ASN1T_LocatedQuantity ) * quantities_.n );
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Clear
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Clear()
{
    if ( quantities_.n > 0 )
    {
        delete [] quantities_.elem;
        quantities_.n = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.toxic_cloudPresent = 1;
    asnMsg.toxic_cloud.quantities.n = quantities_.n;
    asnMsg.toxic_cloud.quantities.elem = quantities_.elem;
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.mine_jam;
}
