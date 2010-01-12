// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Converter.h $
// $Author: Nld $
// $Modtime: 11/08/04 16:10 $
// $Revision: 6 $
// $Workfile: MT_Converter.h $
//
// *****************************************************************************

#ifndef __MT_Converter_h_
#define __MT_Converter_h_

#include "MT_Tools_Types.h"

// =============================================================================
// @class  MT_Converter
// Created: JVT 2004-08-03
// =============================================================================
template< typename KEY, typename VALUE, typename CMP = sCaseInsensitiveLess >
class MT_Converter
{
    MT_COPYNOTALLOWED( MT_Converter )

public:
     MT_Converter( const VALUE& nullValue );
    ~MT_Converter();

    //! @name Operations
    //@{
    bool         Register     ( const KEY& key, const VALUE& value );
    const VALUE& Convert      ( const KEY& key ) const;
    const KEY&   RevertConvert( const VALUE& value ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< class Archive > void load( Archive& archive, const unsigned int version );
    template< class Archive > void save( Archive& archive, const unsigned int version ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef          std::map< KEY, VALUE, CMP > T_Map;   
    typedef typename T_Map::const_iterator       CIT_Map;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > friend void save_construct_data( Archive& archive, const MT_Converter< KEY, VALUE, CMP >* element, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, MT_Converter< KEY, VALUE, CMP >* element, const unsigned int /*version*/ );
    //@}

private:
    const VALUE nullValue_;
    const KEY   nullKey_;
    T_Map       map_;
};

#include "MT_Converter.inl"

#endif // __MT_Converter_h_
