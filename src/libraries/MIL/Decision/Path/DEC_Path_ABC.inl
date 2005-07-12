//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.inl $
// $Author: Age $
// $Modtime: 25/02/05 9:59 $
// $Revision: 2 $
// $Workfile: DEC_Path_ABC.inl $
//
//*****************************************************************************

// =============================================================================
// REFERENCES
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::IncDIARef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
inline
void DEC_Path_ABC::IncDIARef()
{
    ++nNbrDIARefs_;
    if( nNbrDIARefs_ > 1 ) //$$$ TMP pour corriger les scripts
        assert( false );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DecDIARef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
inline
void DEC_Path_ABC::DecDIARef()
{
    if( nNbrDIARefs_ != 1 ) //$$$ TMP pour corriger les scripts
    {
        assert( !"DEC_DetruireItineraire appele plusieurs fois de suite sur le meme itineraire" );
        return;
    }
    --nNbrDIARefs_;
    if( nNbrDIARefs_ == 0 && nNbrRefs_ == 0 )
        delete this; //$$$ !!!
}

//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::IncRef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
inline
void DEC_Path_ABC::IncRef()
{
    ++nNbrRefs_;
}

//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::DecRef
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
inline
void DEC_Path_ABC::DecRef()
{
    assert( nNbrRefs_ > 0 );
    --nNbrRefs_;
    if( nNbrRefs_ == 0 && nNbrDIARefs_ == 0 )
        delete this; //$$$ !!!
}

// =============================================================================
// ACCESSORS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
inline
DEC_Path_ABC::E_State DEC_Path_ABC::GetState() const
{   
    return nState_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
void DEC_Path_ABC::RegisterPathSection( DEC_PathSection& section )
{
    pathSections_.push_back( &section );
}
