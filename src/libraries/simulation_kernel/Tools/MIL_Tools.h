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

#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MT_Tools/MT_String.h"
#include <spatialcontainer/TerrainData.h>
#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )

namespace sword
{
    class CoordLatLong;
}

class MIL_Time_ABC;

enum E_TerrainObjectTypeID;

enum E_KeyPoint
{
    eKeyPointForest = 0,
    eKeyPointUrban,
    eKeyPointCrossroads,
    eKeyPointBridge,
    eKeyPointLima,
    eNbrKeyPoints
};

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-12-02
//=============================================================================
class MIL_Tools : private boost::noncopyable
{
public:
    //-------------------------------------------------------------------------
    /** @name Units converters */
    //-------------------------------------------------------------------------
    //@{
    static double ConvertMeterSquareToSim( double );
    static float ConvertSimToMeterSquare( double );
    static double ConvertMeterToSim( double );
    static float ConvertSimToMeter( double );
    static void ConvertCoordMosToSim( const std::string& strMosPos, MT_Vector2D& vSimPos );
    static void ConvertCoordMosToSim( const sword::CoordLatLong& asnCoordUTM, MT_Vector2D& vSimPos );
    static std::string ConvertCoordSimToMos( const MT_Vector2D& vSimPos );
    static void ConvertCoordSimToMos( const MT_Vector2D& vSimPos, sword::CoordLatLong& asnCoordUTM );

    static double ConvertSpeedMosToSim( double ); // km/h         -> pixel/DeltaT
    static double ConvertSpeedSimToMos( double ); // pixel/DeltaT -> km/h
    static double ConvertSecondsToSim( double ); // s            -> DeltaT
    static double ConvertSecondsToSim( double v, const MIL_Time_ABC& time );
    static double ConvertMinutesToSim( double ); // min          -> DeltaT
    static double ConvertHoursToSim( double ); // h            -> DeltaT
    static double ConvertSimToMinutes( double ); // DeltaT       -> min

    static double GetAltitude( const MT_Vector2D& );
    static const weather::Meteo::sWindData& GetWind( const MT_Vector2D& );
    //@}

    //-------------------------------------------------------------------------
    /** @name name to id converters */
    //-------------------------------------------------------------------------
    //@{
    static E_KeyPoint ConvertLandType( const std::string& sName );
    //@}

    //-------------------------------------------------------------------------
    /** @name id to name converters */
    //-------------------------------------------------------------------------
    //@{
    static const std::string& GetEnvironnementTypeName( PHY_RawVisionData::E_VisionObject );
    static std::string  GetLandTypeName( const TerrainData& data );
    //@}

    //! @name CRC
    //@{
    static boost::crc_32_type::value_type ComputeCRC( const std::string& strFileName );
    static void CheckXmlCrc32Signature( const std::string& filename );
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
        const type type_;
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
        type* type_;
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

        for ( unsigned int i = 0; ; ++i )
        {
            const std::string& sCurrentName = conv[i].name_;
            if( sCurrentName.empty() || sCaseInsensitiveEqual()( sCurrentName, str ) )
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
        for ( unsigned int i = 0; !conv[i].name_.empty(); ++i )
            if( conv[i].type_ == val )
                return conv[i].name_;

        static std::string badString( "" );
        return badString;
    }

private:
    static converter< PHY_RawVisionData::E_VisionObject > environnementConverter_[];

};

#include "MIL_Tools.inl"

#endif // __MIL_Tools_h_
