//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.h $
// $Author: Age $
// $Modtime: 28/02/05 10:04 $
// $Revision: 2 $
// $Workfile: DEC_Path_ABC.h $
//
//*****************************************************************************

#ifndef __DEC_Path_ABC_h_
#define __DEC_Path_ABC_h_

#include "MIL.h"
#include "simulation_terrain/TER_PathFindRequest_ABC.h"

class TER_PathFindReplay;
class DEC_PathSection_ABC;

// =============================================================================
// @class  DEC_Path_ABC
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Path_ABC : public TER_PathFindRequest_ABC
{
    MT_COPYNOTALLOWED( DEC_Path_ABC );

public:
    //! @name Types
    //@{
    enum E_State
    {
        eInvalid    = 0,
        eComputing  = 1,
        eValid      = 2,
        eImpossible = 4,
        ePartial    = 8,
        eCanceled   = 16
    };
    //@}

public:
    //! @name Operations
    //@{
    virtual void Execute              ( TerrainPathfinder& pathfind );
            void Cancel               ();
    virtual void CleanAfterComputation();

            MT_Float GetLength     () const;
    virtual bool     NeedRefine    () const = 0;
    virtual void     AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint ) = 0;
    //@}

    //! @name References
    //@{
    void IncRef();
    void DecRef();

    void IncDIARef();
    void DecDIARef();
    //@}

    //! @name Accessors
    //@{
    uint    GetID   () const;
    E_State GetState() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const DEC_Path_ABC& rhs ) const;
    bool operator!=( const DEC_Path_ABC& rhs ) const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             DEC_Path_ABC();
    virtual ~DEC_Path_ABC();
    //@}

    //! @name Tools
    //@{
    void         RegisterPathSection( DEC_PathSection_ABC& section );
    std::string  GetStateAsString   () const;
    std::string  GetPathAsString    () const;
    virtual void NotifySectionEnded() = 0;
    void         DoExecute( TerrainPathfinder& pathfind );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< DEC_PathSection_ABC* > T_PathSectionVector; 
    typedef T_PathSectionVector::const_iterator CIT_PathSectionVector;
    //@}

private:
    const uint                nID_;
          T_PathSectionVector pathSections_;
          uint                nNbrRefs_;              // nb of references on path
          uint                nNbrDIARefs_;           // nb of references by DIA on path
          E_State             nState_;            
          bool                bJobCanceled_;

private:
    static uint nIDIdx_;
};

#include "DEC_Path_ABC.inl"

#endif // __DEC_Path_ABC_h_