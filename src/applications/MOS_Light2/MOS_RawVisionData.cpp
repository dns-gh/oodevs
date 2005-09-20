//*****************************************************************************
//
// $Created: JVT 02-11-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RawVisionData.cpp $
// $Author: Age $
// $Modtime: 14/04/05 10:29 $
// $Revision: 11 $
// $Workfile: MOS_RawVisionData.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentServer.h"
#include "MOS_RawVisionData.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_SensorType.h"
#include "MOS_Meteo.h"
#include "MOS_Agent.h"

#include "MOS_ASN_Types.h"

#include "MT_Tools/MT_Sector.h"
#include "MOS_Tools.h"

#ifdef max
#   undef max
#endif
#ifdef min
#   undef min
#endif


template< class T > inline const T& max( const T& a, const T& b ) { return a > b ? a : b; }
template< class T > inline const T& min( const T& a, const T& b ) { return a < b ? a : b; }

GFX_Color MOS_RawVisionData::fumigeneColor_ ( 200, 126, 126, 0.8 );
GFX_Color MOS_RawVisionData::eclairantColor_( 255, 255, 255, 0.3 );

const MOS_RawVisionData::sCell MOS_RawVisionData::emptyCell_ = { 0, 0, 0 };

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData constructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
MOS_RawVisionData::MOS_RawVisionData()
    : ppCells_ ( 0 )
    , nNbrCol_ ( 0 )
    , nNbrRow_ ( 0 )
    , pGlobalMeteo_( 0 )
    , nTimeStepMeteoUpdated_( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData destructor
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
MOS_RawVisionData::~MOS_RawVisionData()
{
    if ( ppCells_ )
        Terminate();
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Terminate
// Created: JVT 02-11-20
//-----------------------------------------------------------------------------
void MOS_RawVisionData::Terminate()
{
    assert( ppCells_ );

    for ( uint i = nNbrCol_; i; )
        delete [] ppCells_[--i];
    delete [] ppCells_;
    ppCells_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::InitializeMeteo
// Created: JVT 03-08-08
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_RawVisionData::InitializeMeteo( MOS_InputArchive& archive )
{
    // Initialisation de l'�ph�m�ride
    archive.Section( "Ephemeride" );
    std::string strVal;
    archive.ReadField( "HeureLeverSoleil", strVal );
    
    int hls = 0, mls = 0;
    char tmp = 0;
    {
        std::istringstream strTmp( strVal );
        strTmp >> hls >> tmp >> mls;
    }
    if ( tmp != 'h' || hls < 0 || hls > 23 || mls < 0 || mls > 59 )
        throw MT_ScipioException( "PHY_PerceptionManager::InitializeMeteo", __FILE__, __LINE__, MT_FormatString( "Can't decode time (%d%c%d)", hls, tmp, mls ) );

    archive.ReadField( "HeureCoucherSoleil", strVal );

    int hcs = 0, mcs = 0;
    tmp = 0;
    {
        std::istringstream strTmp( strVal );
        strTmp >> hcs >> tmp >> mcs;
    }
    if ( tmp != 'h' || hcs < 0 || hcs > 23 || mcs < 0 || mcs > 59 )
        throw MT_ScipioException( "PHY_PerceptionManager::InitializeMeteo", __FILE__, __LINE__, MT_FormatString( "Can't decode time (%d%c%d)", hls, tmp, mls ) );

    archive.ReadField( "Lune", strVal );

    E_LightingType nMoon = MOS_Tools::GetLightingFromName( strVal );

    if ( nMoon == (E_LightingType)-1 )
        throw MT_ScipioException( "PHY_PerceptionManager::InitializeMeteo", __FILE__, __LINE__, MT_FormatString( "Unknown moon '%s'", strVal.c_str() ) );

    ephemeride_.Initialize( hls, mls, hcs, mcs, nMoon );
    archive.EndSection();
    
    // Initialisation de la m�t�o globale
    archive.Section( "MeteoGlobale" );
    if ( pGlobalMeteo_ ) pGlobalMeteo_->Destroy();
    pGlobalMeteo_ = MOS_Meteo::Create( archive );
    archive.EndSection();

    // Initialisation des m�t�os locales
    if ( archive.BeginList( "PatchsLocaux", MOS_InputArchive::eNothing ) )
    {
        while ( archive.NextListElement() )
        {
            archive.Section( "PatchLocal" );

            std::string strPos;
            MT_Vector2D vUpLeft;
            MT_Vector2D vDownRight;

            archive.ReadAttribute( "hautGauche", strPos );
            MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vUpLeft );

            archive.ReadAttribute( "basDroit", strPos );
            MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vDownRight );

            MOS_Meteo* pTmp = MOS_Meteo::Create( archive );
            if ( !pTmp )
                throw MT_ScipioException( "PHY_PerceptionManager::InitializeMeteo", __FILE__, __LINE__, "Can't create gloabl meteo");

            RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );

            archive.EndSection();
        }
        archive.EndList();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::RegisterMeteoPatch
// Created: JVT 03-08-06
// Last modified: JVT 03-08-18
//-----------------------------------------------------------------------------
void MOS_RawVisionData::RegisterMeteoPatch( const MT_Vector2D& upLeft, const MT_Vector2D& downRight, MOS_Meteo* pMeteo )
{
    uint nXEnd = min( GetCol( downRight.rX_ ), nNbrCol_ - 1 );
    uint nYEnd = min( GetRow( upLeft.rY_ ),    nNbrRow_ - 1 );
    uint nXBeg = min( GetCol( upLeft.rX_ ),    nNbrCol_ - 1 );
    uint nYBeg = min( GetRow( downRight.rY_ ), nNbrRow_ - 1 );
    
    // On remet �ventuellement dans le bon sens
    if ( nXEnd < nXBeg )
        std::swap( nXEnd, nXBeg );
    if ( nYEnd < nYBeg )
        std::swap( nYEnd, nYBeg );

    // enregistrement du nombre de r�f�rence pour cette m�t�o
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
    nTimeStepMeteoUpdated_ = MOS_App::GetApp().GetTime();
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::UpdateEphemeride
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_RawVisionData::UpdateEphemeride()
{
    if ( ephemeride_.UpdateNight() )
    {
        nTimeStepMeteoUpdated_ = MOS_App::GetApp().GetTime();
        MOS_Meteo::UpdateAllMeteo( ephemeride_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::AlignToCell
// Created: JVT 02-11-21
//-----------------------------------------------------------------------------
void MOS_RawVisionData::AlignToCell( MT_Vector2D& pos, E_AlignType type ) const
{
    // Recherche de la cellule concern�e
    pos.rX_ = rCellSize_ * GetCol( pos.rX_ );
    pos.rY_ = rCellSize_ * GetRow( pos.rY_ );

    switch( type )
    {
        case eTopLeft:
        {
            pos.rY_ += rCellSize_;
            return;
        }        
        case eTopRight:
        {
            pos.rX_ += rCellSize_;
            pos.rY_ += rCellSize_;
            return;
        }        
        case eBottomLeft:
        {
            return;
        }        
        case eBottomRight:
        {
            pos.rX_ += rCellSize_;
            return;
        }
        case eCenter:
        {
            pos.rX_ += rCellSize_ / 2.;
            pos.rY_ += rCellSize_ / 2.;
            return;
        }        
    }
}


//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Read
// Created: JVT 02-12-11
//-----------------------------------------------------------------------------
bool MOS_RawVisionData::Read( const std::string& strFile )
{
    try
    {
        MT_FlatBinaryInputArchive archive;
        archive.EnableExceptions( true );
        if ( !archive.Open( strFile ) )
            return false;

        if (   !archive.ReadField( "rCellSize_", rCellSize_ )
            || !archive.ReadField( "nNbrRow_"  , nNbrRow_ )
            || !archive.ReadField( "nNbrCol_"  , nNbrCol_ )
            )
            return false;

        assert( !ppCells_ );
        ppCells_ = new sCell*[ nNbrCol_ ];

        for ( uint x = 0; x < nNbrCol_; ++x )
        {
            sCell* pTmp = new sCell[ nNbrRow_ ];
            ppCells_[ x ] = pTmp;

            for ( uint i = 0; i < nNbrRow_; ++i )
            {
                pTmp->pMeteo = 0;
                if ( !archive.ReadData( 4, (char*)pTmp++ ) )
                    return false;
            }
        }
    }
    catch( ... )
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::CalcMinMaxAltitude
// Created: FBD 03-02-13
// Last modified: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_RawVisionData::CalcMinMaxAltitude()
{
    short nMaxAltitude = std::numeric_limits< short >::min();
    short nMinAltitude = std::numeric_limits< short >::max();

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


// -----------------------------------------------------------------------------
// Name: MOS_RawVisionData::UpdateVisionMatrice
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
void MOS_RawVisionData::UpdateVisionMatrice( T_VisionResultMap& res, const MOS_SensorType& sensorType, const MT_Sector& sector, MT_Float rHeight, const MOS_Agent& agent ) const
{
    const MT_Float rRadius = sensorType.GetMaxDistance( agent );

    // recherche des bornes des cases � tester
    // $$$$ JVT : on prend un carr� englobant le cercle max.... sachant qu'on connait les caract�ristiques du secteur, on peut faire beaucoup plus pr�cis ...
    const MT_Vector2D& origin      = sector.GetOrigin();
    const MT_Rect&     worldBounds = MOS_App::GetApp().GetWorld().GetRect();
          MT_Vector2D  current     ( max( worldBounds.GetLeft(), origin.rX_ - rRadius ), max( worldBounds.GetBottom(), origin.rY_ - rRadius ) );
          MT_Vector2D  end         ( min( worldBounds.GetRight() , origin.rX_ + rRadius ), min( worldBounds.GetTop(), origin.rY_ + rRadius ) );

    // parcours des cases possibles pour remplir la matrice
    const MT_Float rYMem = current.rY_;
    const MT_Float rStep = MOS_App::GetApp().GetRawVisionData().GetCellSize();

    for ( ; current.rX_ <= end.rX_; current.rX_ += rStep, current.rY_ = rYMem )
        for ( ; current.rY_ <= end.rY_; current.rY_ += rStep )
        {
            if ( !sector.IsInCone( current, rRadius ) )
                continue;

            E_PerceptionResult& perceptionResult = res[ current ];

            if ( perceptionResult != eIdentification )
                perceptionResult = max( perceptionResult, FindPerception( agent, sensorType, current, rHeight ) );
        }
}

// -----------------------------------------------------------------------------
// Name: MOS_RawVisionData::FindPerception
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
E_PerceptionResult MOS_RawVisionData::FindPerception( const MOS_Agent& agent, const MOS_SensorType& sensorType, const MT_Vector2D& vTargetPos, MT_Float rHeight ) const
{
    const MT_Vector2D& vSourcePos      = agent.GetPos();
    const MT_Float     rSourceAltitude = GetHeight( vSourcePos ) + agent.GetAltitude() + rHeight;
    const MT_Float     rTargetAltitude = GetHeight( vTargetPos ) + 2;

    Iterator it( MT_Vector3D( vSourcePos.rX_, vSourcePos.rY_, rSourceAltitude ), MT_Vector3D( vTargetPos.rX_, vTargetPos.rY_, rTargetAltitude ) );

    MT_Float rVisionNRJ = it.End() ? std::numeric_limits< MT_Float >::max() : sensorType.ComputeExtinction( it, agent );

    while ( rVisionNRJ > 0 && !(++it).End() )
        rVisionNRJ = sensorType.ComputeExtinction( it, agent, rVisionNRJ );

    return sensorType.InterpreteNRJ( rVisionNRJ );
}

//=============================================================================
// DEBUG
//=============================================================================

bool vector2DLessXYScaled::operator () ( const MT_Vector2D& rhs, const MT_Vector2D& lhs ) const
{
    MT_Vector2D scaledRhs( MOS_App::GetApp().GetRawVisionData().GetRow( rhs.rX_ ), MOS_App::GetApp().GetRawVisionData().GetCol( rhs.rY_ ) );
    MT_Vector2D scaledLhs( MOS_App::GetApp().GetRawVisionData().GetRow( lhs.rX_ ), MOS_App::GetApp().GetRawVisionData().GetCol( lhs.rY_ ) );

    return vector2DLessXY::operator () ( scaledRhs, scaledLhs );
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::UpdateGlobalMeteo
// Created: JVT 03-08-08
//-----------------------------------------------------------------------------
void MOS_RawVisionData::UpdateGlobalMeteo( const ASN1T_MeteoAttributs& attrs )
{
    nTimeStepMeteoUpdated_ = MOS_App::GetApp().GetTime();
    pGlobalMeteo_->Terminate();
    pGlobalMeteo_->Initialize( attrs );
}

// -----------------------------------------------------------------------------
// Name: MOS_RawVisionData::IsMeteoUpdated
// Created: NLD 2003-10-29
// -----------------------------------------------------------------------------
bool MOS_RawVisionData::IsMeteoUpdated() const
{
    return nTimeStepMeteoUpdated_ == MOS_App::GetApp().GetTime();
}




//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::RegisterAmmoMeteoEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_RawVisionData::RegisterAmmoMeteoEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    MT_Vector2D point1, point2, point3;

    assert( asnMsg.localisation.vecteur_point.n == 3 );

    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[0].data, point1 );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[1].data, point2 );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[2].data, point3 );
    
    MT_Ellipse* pEllipse = new MT_Ellipse( point1, point2, point3 );
    ammoMeteoEffects_[ asnMsg.oid_effet ] = std::make_pair( 
                                                            asnMsg.type == EnumFireEffectType::fumigene ? &fumigeneColor_ : &eclairantColor_,
                                                            pEllipse
                                                          );
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::UnregisterAmmoMeteoEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_RawVisionData::UnregisterAmmoMeteoEffect( uint nID )
{
    std::map< uint, std::pair< const GFX_Color*, const MT_Ellipse* > >::iterator it = ammoMeteoEffects_.find( nID );

    if ( it != ammoMeteoEffects_.end() )
    {
        delete it->second.second;
        ammoMeteoEffects_.erase( it );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::DrawAmmoMeteoEffects
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_RawVisionData::DrawAmmoMeteoEffects() const
{
    for ( std::map< uint, std::pair< const GFX_Color*, const MT_Ellipse* > >::const_iterator it = ammoMeteoEffects_.begin(); it != ammoMeteoEffects_.end(); ++it )
    {
        const GFX_Color&  color   = *it->second.first;
        const MT_Ellipse& ellipse = *it->second.second;

        const MT_Rect bb = ellipse.GetBoundingBox();
        MT_Float x          = floor( bb.GetLeft()   / rCellSize_ ) * rCellSize_;
        const MT_Float xMax = ceil ( bb.GetRight()  / rCellSize_ ) * rCellSize_;
        const MT_Float yMin = floor( bb.GetBottom() / rCellSize_ ) * rCellSize_;
        const MT_Float yMax = ceil ( bb.GetTop()    / rCellSize_ ) * rCellSize_;

        for ( ; x < xMax; x+=rCellSize_ )
        {
            for ( MT_Float y = yMin; y < yMax; y+=rCellSize_ )
            {
                if ( ellipse.IsInside( MT_Vector2D( x, y ) ) )
                    GFX_Tools::CreateGLRectPoly( MT_Rect( x, y, x+rCellSize_, y+rCellSize_ ), color );
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Getcol
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
uint MOS_RawVisionData::GetCol( double x ) const
{
    return (uint)( ( x - MOS_App::GetApp().GetWorld().GetRect().GetLeft() ) / rCellSize_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_RawVisionData::Getrow
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
uint MOS_RawVisionData::GetRow( double y ) const
{
    return (uint)( ( y - MOS_App::GetApp().GetWorld().GetRect().GetBottom() ) / rCellSize_);
}
