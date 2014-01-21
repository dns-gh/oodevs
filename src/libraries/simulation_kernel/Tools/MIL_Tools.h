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

#include <spatialcontainer/TerrainData.h>

namespace tools
{
    class Path;
}

namespace sword
{
    class CoordLatLong;
}

namespace weather
{
    struct WindData;
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
    eNbrKeyPoint
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
    static unsigned int ConvertSpeedSimToMos( double ); // pixel/DeltaT -> km/h
    static double ConvertSecondsToSim( double ); // s            -> DeltaT
    static double ConvertSecondsToSim( double v, const MIL_Time_ABC& time );
    static double ConvertMinutesToSim( double ); // min          -> DeltaT
    static double ConvertHoursToSim( double ); // h            -> DeltaT
    static double ConvertSimToMinutes( double ); // DeltaT       -> min

    static double GetAltitude( const MT_Vector2D& );
    static const weather::WindData& GetWind( const MT_Vector2D& );
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
    static std::string  GetLandTypeName( const TerrainData& data );
    //@}

private:
    MIL_Tools();
};

#endif // __MIL_Tools_h_
