// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Converter.inl $
// $Author: Nld $
// $Modtime: 11/08/04 16:17 $
// $Revision: 1 $
// $Workfile: MT_Converter.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MT_Converter constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
MT_Converter<KEY, VALUE, CMP>::MT_Converter( const VALUE& nullValue )
: nullValue_( nullValue )
{
}

// -----------------------------------------------------------------------------
// Name: MT_Converter destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
MT_Converter<KEY, VALUE, CMP>::~MT_Converter()
{
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::Register
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
bool MT_Converter<KEY, VALUE, CMP>::Register( const KEY& key, const VALUE& value )
{
    return map_.insert( std::make_pair( key, value ) ).second;
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::Convert
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
const VALUE& MT_Converter<KEY, VALUE, CMP>::Convert( const KEY& key )
{
    CIT_Map it = map_.find( key );
    if( it == map_.end() )
        return nullValue_;
    return it->second;
}
