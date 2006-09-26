// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
template< typename T > inline
ModelRefsContainer< T >::ModelRefsContainer()
{
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
template< typename T > inline
ModelRefsContainer< T >::~ModelRefsContainer()
{
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer::Clear
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelRefsContainer< T >::Clear()
{
    modelRefs_.clear();
}

// -----------------------------------------------------------------------------
// Name: template< typename U > void ModelRefsContainer::Register
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelRefsContainer< T >::Register( T& t )
{
    if( !modelRefs_.insert( std::make_pair( t.GetID(), &t ) ).second )
        ThrowError( t.GetID(), "already exists" );
}

// -----------------------------------------------------------------------------
// Name: template< typename U > void ModelRefsContainer::Unregister
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelRefsContainer< T >::Unregister( T& t )
{
    if( modelRefs_.erase( t.GetID() ) == 0 )
        ThrowError( t.GetID(), "doesn't exist" );
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer::ThrowError
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelRefsContainer< T >::ThrowError( unsigned long nID, const std::string& strMessage ) const
{
    std::stringstream str;
    str << "Ref element type '" << typeid( T ).name() << "' '" << nID << "' " << strMessage;
    throw std::runtime_error( str.str() );

}

}