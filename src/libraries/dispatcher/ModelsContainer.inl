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
ModelsContainer< T >::ModelsContainer()
{
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
template< typename T > inline
ModelsContainer< T >::~ModelsContainer()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Create
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > template< typename P > inline
T& ModelsContainer< T >::Create( Model& model, unsigned long nID, P& param )
{
    T*& pModel = models_[ nID ];
    if( pModel )
        return *pModel;

    pModel = new T( model, param );
//    MT_LOG_INFO_MSG( "New element type '" << typeid( T ).name() << "' '" << nID << "' " );
    return *pModel;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Create
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > template< typename P1, typename P2 > inline
T& ModelsContainer< T >::Create( Model& model, unsigned long nID, P1& param1, P2& param2 )
{
    T*& pModel = models_[ nID ];
    if( pModel )
        return *pModel;

    pModel = new T( model, param1, param2 );
//    MT_LOG_INFO_MSG( "New element type '" << typeid( T ).name() << "' '" << nID << "' " );
    return *pModel;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Destroy
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelsContainer< T >::Destroy( unsigned long nID )
{
    IT_ModelMap it = models_.find( nID );
    if( it == models_.end() )
        ThrowError( nID, "doesn't exist" );
    delete it->second;
    //MT_LOG_INFO_MSG( "Delete element type '" << typeid( T ).name() << "' '" << nID << "' " );
    models_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Find
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
T& ModelsContainer< T >::Get( unsigned long nID ) const
{
    CIT_ModelMap it = models_.find( nID );
    if( it == models_.end() )
        ThrowError( nID, "does not exist" );
    return *it->second;
}


// -----------------------------------------------------------------------------
// Name: ModelsContainer::Find
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
T* ModelsContainer< T >::Find( unsigned long nID ) const
{
    CIT_ModelMap it = models_.find( nID );
    if( it == models_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Apply
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
template< typename T > template< typename F > inline
void ModelsContainer< T >::Apply( F functor ) const
{
    for( CIT_ModelMap it = models_.begin(); it != models_.end(); ++it )
        functor( *it->second );
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Apply
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
template< typename T > template< typename F, typename P1 > inline
void ModelsContainer< T >::Apply( F functor, P1& param1 ) const
{
    for( CIT_ModelMap it = models_.begin(); it != models_.end(); ++it )
        functor( *it->second, param1 );
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::ThrowError
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelsContainer< T >::ThrowError( unsigned long nID, const std::string& strMessage ) const
{
    std::stringstream str;
    str << "Element type '" << typeid( T ).name() << "' '" << nID << "' " << strMessage;
    throw std::runtime_error( str.str() );
}

// -----------------------------------------------------------------------------
// Name: template< typename L, typename E > ModelsContainer::SerializeTest
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
template< typename T > template< typename L, typename E > inline
void ModelsContainer< T >::Send( L& asnList ) const
{
    asnList.n = models_.size();
    if( !models_.empty() )
    {
        asnList.elem = new E[ models_.size() ];
        uint i = 0;
        for( CIT_ModelMap it = models_.begin(); it != models_.end(); ++it, ++i )
            it->second->Send( asnList.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer::Clear
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
template< typename T > inline
void ModelsContainer< T >::Clear()
{
    for( CIT_ModelMap it = models_.begin(); it != models_.end(); ++it )
        delete it->second;      
    models_.clear();
}

}