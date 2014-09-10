// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeProxy_ABC_h_
#define __DEC_Knowledge_ObjectAttributeProxy_ABC_h_

#include "DEC_Knowledge_IObjectAttributeProxy.h"

// =============================================================================
// Created: NLD 2010-10-26
// =============================================================================
template< typename T >
class DEC_Knowledge_ObjectAttributeProxy_ABC : public DEC_Knowledge_IObjectAttributeProxy
{
public:
    DEC_Knowledge_ObjectAttributeProxy_ABC();
    virtual ~DEC_Knowledge_ObjectAttributeProxy_ABC();

    //! @name Accessors
    //@{
    const T* GetAttribute() const { return pAttribute_; }
    //@}

    //! @name Operations
    //@{
    template< typename Source >
    bool ForceUpdateAttributeFromObject( const Source& source );
    virtual bool CopyFrom( const DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( sword::ObjectAttributes& asn ) const;
    virtual void SendFullState   ( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive >
    void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< DEC_Knowledge_IObjectAttributeProxy >( *this );
        ar & pAttribute_;

    }
    virtual void Register( DEC_Knowledge_Object& knowledge );
    //@}

protected:
    //! @name Tools
    //@{
    template< typename Source >
    bool UpdateAttributeFromSource( const Source& source );
    //@}

private:
    T* pAttribute_;
};

// =============================================================================
// Implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC constructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
DEC_Knowledge_ObjectAttributeProxy_ABC< T >::DEC_Knowledge_ObjectAttributeProxy_ABC()
    : pAttribute_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC destructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
DEC_Knowledge_ObjectAttributeProxy_ABC< T >::~DEC_Knowledge_ObjectAttributeProxy_ABC()
{
    delete pAttribute_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::UpdateAttributeFromSource
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
template< typename Source >
bool DEC_Knowledge_ObjectAttributeProxy_ABC< T >::UpdateAttributeFromSource( const Source& source )
{
    const T* pNewData = source.GetAttribute< T >();
    if( !pNewData )
        return false;

    if( !pAttribute_ )
        pAttribute_ = new T();
    return pAttribute_->Update( *pNewData );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::ForceUpdateAttributeFromObject
// Created: NPT 2012-18-12
// -----------------------------------------------------------------------------
template< typename T >
template< typename Source >
bool DEC_Knowledge_ObjectAttributeProxy_ABC< T >::ForceUpdateAttributeFromObject( const Source& source )
{
    const T* pNewData = source.RetrieveAttribute< T >();
    if( !pNewData )
        return false;

    if( !pAttribute_ )
        pAttribute_ = new T();
    return pAttribute_->Update( *pNewData );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::CopyFrom
// Created: JSR 2013-12-06
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxy_ABC< T >::CopyFrom( const DEC_Knowledge_Object& knowledge )
{
    const T* pNewData = knowledge.RetrieveAttribute< T >();
    if( !pNewData )
        return false;
    if( !pAttribute_ )
        pAttribute_ = new T();
    *pAttribute_ = *pNewData;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::SendChangedState
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_ObjectAttributeProxy_ABC< T >::SendChangedState( sword::ObjectAttributes& asn ) const
{
    const T* pData = GetAttribute();
    if( pData )
        pData->SendUpdate( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::SendFullState
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_ObjectAttributeProxy_ABC< T >::SendFullState( sword::ObjectAttributes& asn ) const
{
    const T* pData = GetAttribute();
    if( pData )
        pData->SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxy_ABC::Register
// Created: NLD 2010-11-03
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_ObjectAttributeProxy_ABC< T >::Register( DEC_Knowledge_Object& knowledge )
{
    knowledge.Attach( *this );
}

#endif // __DEC_Knowledge_ObjectAttributeProxy_ABC_h_
