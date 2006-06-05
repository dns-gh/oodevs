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
    const VALUE& Convert      ( const KEY& key );
    const KEY&   RevertConvert( const VALUE& value );
    //@}

private:
    //! @name Types
    //@{
    typedef          std::map< KEY, VALUE, CMP > T_Map;   
    typedef typename T_Map::const_iterator       CIT_Map;
    //@}

private:
    const VALUE nullValue_;
    const KEY   nullKey_;
    T_Map       map_;
};

#include "MT_Converter.inl"

#endif // __MT_Converter_h_
