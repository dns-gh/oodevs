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
MT_Converter< KEY, VALUE, CMP >::MT_Converter( const VALUE& nullValue )
    : nullValue_( nullValue )
    , nullKey_  ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_Converter destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
MT_Converter< KEY, VALUE, CMP >::~MT_Converter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::Register
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
bool MT_Converter< KEY, VALUE, CMP >::Register( const KEY& key, const VALUE& value )
{
    return map_.insert( std::make_pair( key, value ) ).second;
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::Convert
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
const VALUE& MT_Converter< KEY, VALUE, CMP >::Convert( const KEY& key ) const
{
    CIT_Map it = map_.find( key );
    if( it == map_.end() )
        return nullValue_;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::RevertConvert
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
const KEY& MT_Converter< KEY, VALUE, CMP >::RevertConvert( const VALUE& value ) const
{
    for( auto it = map_.begin(); it != map_.end(); ++it )
        if( it->second == value )
            return it->first;
    return nullKey_;
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void MT_Converter::load
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
template< class Archive >
void MT_Converter< KEY, VALUE, CMP >::load( Archive& archive, const unsigned int /*version*/ )
{
    std::size_t count;
    archive >> count;
    while( count-- )
    {
        KEY key;
        archive >> key;
        archive >> map_[ key ];
    }
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void MT_Converter::save
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
template< typename KEY, typename VALUE, typename CMP >
template< class Archive >
void MT_Converter< KEY, VALUE, CMP >::save( Archive& archive, const unsigned int /*version*/ ) const
{
    std::size_t size = map_.size();
    archive << size;
    for( auto it = map_.begin(); it != map_.end(); ++it )
    {
        archive << it->first;
        archive << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::save_construct_data
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Archive, typename KEY, typename VALUE, typename CMP >
void save_construct_data( Archive& archive, const MT_Converter< KEY, VALUE, CMP >* element, const unsigned int  )
{
    archive << element->nullValue_;
}

// -----------------------------------------------------------------------------
// Name: MT_Converter::load_construct_data
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Archive, typename KEY, typename VALUE, typename CMP >
void load_construct_data( Archive& archive, MT_Converter< KEY, VALUE, CMP >* element, const unsigned int  )
{
    VALUE value;
    archive >> value;
    ::new( element ) MT_Converter< KEY, VALUE, CMP >( value );
}
