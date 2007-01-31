//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Tools.h $
// $Author: Jvt $
// $Modtime: 12/04/05 11:59 $
// $Revision: 17 $
// $Workfile: MIL_Tools.h $
//
//*****************************************************************************

#ifndef __MIL_Tools_h_
#define __MIL_Tools_h_

#include "MIL.h"

#include "game_asn/Asn.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"

#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )

enum E_TerrainObjectTypeID;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-12-02
//=============================================================================
class MIL_Tools
{
    MT_COPYNOTALLOWED( MIL_Tools );

public:
    //-------------------------------------------------------------------------
    /** @name Units converters */
    //-------------------------------------------------------------------------
    //@{    
    static MT_Float    ConvertMeterSquareToSim( MT_Float );
    static float       ConvertSimToMeterSquare( MT_Float );
    static MT_Float    ConvertMeterToSim      ( MT_Float );
    static float       ConvertSimToMeter      ( MT_Float );
    static void        ConvertCoordMosToSim   ( const std::string& strMosPos, MT_Vector2D& vSimPos );
    static void        ConvertCoordMosToSim   ( const ASN1T_CoordUTM& asnCoordUTM, MT_Vector2D& vSimPos );
    static std::string ConvertCoordSimToMos   ( const MT_Vector2D& vSimPos );

    static MT_Float ConvertSpeedMosToSim( MT_Float ); // km/h         -> pixel/DeltaT
    static MT_Float ConvertSpeedSimToMos( MT_Float ); // pixel/DeltaT -> km/h
    static MT_Float ConvertSecondsToSim ( MT_Float ); // s            -> DeltaT
    static MT_Float ConvertMinutesToSim ( MT_Float ); // min          -> DeltaT
    static MT_Float ConvertHoursToSim   ( MT_Float ); // h            -> DeltaT
    static MT_Float ConvertSimToMinutes ( MT_Float ); // DeltaT       -> min
    
    static       MT_Float              GetAltitude( const MT_Vector2D& );
    static const PHY_Meteo::sWindData& GetWind    ( const MT_Vector2D& );   
    //@}

    //-------------------------------------------------------------------------
    /** @name name to id converters */
    //-------------------------------------------------------------------------
    //@{
    static TerrainData ConvertLandType( const std::string& );
    //@}

    //-------------------------------------------------------------------------
    /** @name id to name converters */
    //-------------------------------------------------------------------------
    //@{
    static const std::string& GetEnvironnementTypeName( PHY_RawVisionData::E_VisionObject );
    static       std::string  GetLandTypeName         ( const TerrainData& data );
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    static uint  GetRealTime();
    static float ConvertRealTimeSimToDia( uint nSimRealTime );
    static uint  ConvertRealTimeDiaToSim( float nDiaRealTime );
    //@}
    
    //! @name CRC
    //@{
    static boost::crc_32_type::value_type ComputeCRC( const std::string& strFileName );
    //@}
    
private:
    MIL_Tools();

private:
    //-----------------------------------------------------------------------------
    // Name: struct converter
    // Created: JVT 03-01-08
    //-----------------------------------------------------------------------------
    template < typename type > struct converter
    {
        const std::string name_;
        const type        type_;

        converter( const char* name, type t ) : name_( name ), type_( t ) {};
        converter& operator = ( const converter& rhs ) { name_ = rhs.name_; type_ = rhs.type_; return *this;};
    };

    //-----------------------------------------------------------------------------
    // Name: struct converter
    // Created: JVT 03-02-27
    //-----------------------------------------------------------------------------
    template< typename type > struct mutableConverter
    {
        const std::string name_;
        type*             type_;

        mutableConverter( const char* name, type& t ) : name_( name ), type_( &t ) {};
        mutableConverter& operator = ( const mutableConverter& rhs ) { name_ = rhs.name_; type_ = rhs.type_; return *this;};
    };

    template< typename T > static T BadId() { return T( -1 ); };
    template<> static std::string BadId< std::string >() { return ""; }
    template<> static TerrainData BadId< TerrainData >() { return TerrainData(); }

    //-----------------------------------------------------------------------------
    // Name: FindInConverter
    // Created: JVT 03-01-08
    // Last modified: JVT 03-09-29
    //-----------------------------------------------------------------------------
    template < typename T > static const T& FindInConverter( const converter<T> conv[], const std::string& str )
    {
        static T badId = BadId< T >();

        for ( uint i = 0; ; ++i )
        {
            const std::string& sCurrentName = conv[i].name_;

            if ( sCurrentName.empty() || sCaseInsensitiveEqual()( sCurrentName, str ) )
                return conv[i].type_;
        }
        /*assert( false ); // On ne doit pas arriver ici !!!
        return badId;*/
    }

    //-----------------------------------------------------------------------------
    // Name: InverseFindInConverter
    // Created: JVT 03-04-29
    //-----------------------------------------------------------------------------
    template < typename T > static const std::string& InverseFindInConverter( const converter<T> conv[], T val )
    {
        for ( uint i = 0; !conv[i].name_.empty(); ++i )
            if ( conv[i].type_ == val )
                return conv[i].name_;

        static std::string badString( "" );
        return badString;
    }

    //! @name Tools
    //@{
    static MT_Float GetMeterPerPixel            ();
    static MT_Float GetMeterSquarePerPixelSquare();
    //@}

private:
    static converter< PHY_RawVisionData::E_VisionObject > environnementConverter_[];
  
};

#include "MIL_Tools.inl"

#endif // __MIL_Tools_h_
