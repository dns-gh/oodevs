//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/RawVisionData/PHY_RawVisionData.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 10:32 $
// $Revision: 12 $
// $Workfile: PHY_RawVisionData.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RawVisionData.h"

#include "simulation_terrain/TER_Localisation.h"
#include "PHY_AmmoEffect.h"
#include "MIL_AgentServer.h"

#include "MT_Tools/MT_Rect.h"
#include "MT_Tools/MT_Ellipse.h"
#include "xeumeuleu/xml.h"
#include "tools/InputBinaryStream.h"

      PHY_RawVisionData::sCell PHY_RawVisionData::emptyCell_;
const PHY_Meteo*               PHY_RawVisionData::sCell::pGlobalMeteo_;


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::sCell::GetPrecipitation
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const PHY_Precipitation& PHY_RawVisionData::sCell::GetPrecipitation() const
{ 
    const PHY_Precipitation& mainPrecipitation = pMeteo ? pMeteo->GetPrecipitation() : pGlobalMeteo_->GetPrecipitation();
    return pEffects ? pEffects->GetPrecipitation( mainPrecipitation ) : mainPrecipitation;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetLighting
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const PHY_Lighting& PHY_RawVisionData::sCell::GetLighting() const
{
    const PHY_Lighting& mainLighting = pMeteo ? pMeteo->GetLighting() : pGlobalMeteo_->GetLighting();
    return pEffects ? pEffects->GetLighting( mainLighting ) : mainLighting; 
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData constructor
// Created: JVT 02-11-05
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
PHY_RawVisionData::PHY_RawVisionData( PHY_Meteo& globalMeteo, MIL_Config& config )
    : ppCells_( 0 )
    , nNbrCol_( 0 )
    , nNbrRow_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing vision data" );

    xml::xifstream xis( config.GetTerrainFile() );
    std::string strRawVisionDirectory;
    xis >> xml::start( "Terrain" )
        >> xml::content( "RawVision", strRawVisionDirectory );

    std::string detection = config.BuildTerrainChildFile( strRawVisionDirectory + "/detection.dat" );
    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( detection );
    Read( detection );

    sCell::pGlobalMeteo_ = &globalMeteo;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData destructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
PHY_RawVisionData::~PHY_RawVisionData()
{
    for ( uint i = nNbrCol_; i; )
        delete [] ppCells_[--i];
    delete [] ppCells_;
    ppCells_ = 0;
}


//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterMeteoPatch
// Created: JVT 03-08-06
// Last modified: JVT 03-08-18
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterMeteoPatch( const MT_Vector2D& upLeft, const MT_Vector2D& downRight, PHY_Meteo* pMeteo )
{
    assert( ppCells_ );
    uint nXEnd = std::min( GetCol( downRight.rX_ ), nNbrCol_ - 1 );
    uint nYEnd = std::min( GetRow( upLeft.rY_ ),    nNbrRow_ - 1 );
    uint nXBeg = std::min( GetCol( upLeft.rX_ ),    nNbrCol_ - 1 );
    uint nYBeg = std::min( GetRow( downRight.rY_ ), nNbrRow_ - 1 );
    
    // On remet éventuellement dans le bon sens
    if ( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if ( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    // enregistrement du nombre de référence pour cette météo
    if ( pMeteo )
        pMeteo->IncRef( ( nXEnd - nXBeg + 1 ) * ( nYEnd - nYBeg + 1 ) );

    while ( nXBeg <= nXEnd )
    {
        for ( uint y = nYBeg; y <= nYEnd; ++y )
        {
            sCell& cell = ppCells_[ nXBeg ][ y ];
            if ( cell.pMeteo )
                cell.pMeteo->DecRef();
            cell.pMeteo = pMeteo;
        }
        ++nXBeg;
    }
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::RegisterWeatherEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
void PHY_RawVisionData::RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponClass )
{
    const MT_Rect bb = surface.GetBoundingBox();
    MT_Float x          = floor( bb.GetLeft()   / rCellSize_ ) * rCellSize_;
    const MT_Float xMax = ceil ( bb.GetRight()  / rCellSize_ ) * rCellSize_;
    const MT_Float yMin = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
    const MT_Float yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;
    
    for ( ; x < xMax; x+=rCellSize_ )
        for ( MT_Float y = yMin; y < yMax; y+=rCellSize_ )
            if ( surface.IsInside( MT_Vector2D( x, y ) ) )
            {
                sCell& cell = operator () ( x, y );
                if ( &cell == &emptyCell_ )
                    continue;

                PHY_AmmoEffect* pEffect = new PHY_AmmoEffect( weaponClass, cell.pEffects );
                cell.pEffects = pEffect;
            }
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::UnregisterWeatherEffect
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
void PHY_RawVisionData::UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& weaponClass )
{
    const MT_Rect bb = surface.GetBoundingBox();
    MT_Float x          = floor( bb.GetLeft()   / rCellSize_ ) * rCellSize_;
    const MT_Float xMax = ceil ( bb.GetRight()  / rCellSize_ ) * rCellSize_;
    MT_Float y          = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
    const MT_Float yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;
    
    for ( ; x < xMax; x+=rCellSize_ )
        for ( ; y < yMax; y+=rCellSize_ )
            if ( surface.IsInside( MT_Vector2D( x, y ) ) )
            {
                sCell& cell = operator () ( x, y );
                if ( &cell == &emptyCell_ )
                    continue;

                PHY_AmmoEffect* pPrevEffect = 0;
                PHY_AmmoEffect* pEffect = cell.pEffects;

                while ( pEffect && !pEffect->HandleAmmo( weaponClass ) )
                {
                    pPrevEffect = pEffect;
                    pEffect     = pEffect->GetNextEffect();
                }

                if ( !pEffect || !pEffect->HandleAmmo( weaponClass ) )
                {
                    assert( false );
                    return;
                }

                if ( pPrevEffect )
                    pPrevEffect->SetNextEffect( pEffect->GetNextEffect() );
                else
                    cell.pEffects = pEffect->GetNextEffect();

                delete pEffect;
            }
}



//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::Read
// Created: JVT 02-12-11
// Last modified: JVT 04-03-24
//-----------------------------------------------------------------------------
bool PHY_RawVisionData::Read( const std::string& strFile )
{
    tools::InputBinaryStream archive( strFile.c_str() );
    if ( !archive )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file %s", strFile.c_str() ) );

    if ( ! ( archive >> rCellSize_ >> nNbrRow_ >> nNbrCol_ ) )
       throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Error reading file %s", strFile.c_str() ) );

    assert( !ppCells_ );
    ppCells_ = new sCell*[ nNbrCol_ ];

    for ( uint x = 0; x < nNbrCol_; ++x )
    {
        sCell* pTmp = new sCell[ nNbrRow_ ];
        ppCells_[ x ] = pTmp;

        for ( uint i = 0; i < nNbrRow_; ++i )
        {
            pTmp->pMeteo   = 0;
            pTmp->pEffects = 0;
            archive.Read( (char*)pTmp++, 4 );
            if ( !archive )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Error reading file %s", strFile.c_str() ) );
        }
    }

    CalcMinMaxAltitude();
    return true;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::CalcMinMaxAltitude
// Created: FBD 03-02-13
// Last modified: JVT 03-06-05
//-----------------------------------------------------------------------------
void PHY_RawVisionData::CalcMinMaxAltitude()
{
    short nMaxAltitude = std::numeric_limits<short>::min();
    short nMinAltitude = std::numeric_limits<short>::max();

   for ( uint nX = 0; nX < nNbrCol_ ; ++nX  )
    {
        for ( uint nY = 0; nY < nNbrRow_ ; ++nY )
        {
            short nAltitude = ppCells_[nX][nY].h;
            if ( nAltitude < nMinAltitude )
                nMinAltitude = nAltitude;
            if ( nAltitude > nMaxAltitude )
                nMaxAltitude = nAltitude;
        }
    }

   rMaxAltitude_ = (MT_Float)nMaxAltitude;
   rMinAltitude_ = (MT_Float)nMinAltitude;
}
