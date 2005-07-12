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
#include "TER/TER_PathFindRequest_ABC.h"

class TER_PathFindReplay;
class DEC_PathSection;

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
    virtual void Execute( TerrainPathfinder& pathfind );
            void Cancel ();
    MT_Float GetLength() const;
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
    E_State GetState() const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             DEC_Path_ABC();
    virtual ~DEC_Path_ABC();
    //@}

    //! @name Tools
    //@{
    void        RegisterPathSection( DEC_PathSection& section );
    std::string GetStateAsString   () const;
    std::string GetPathAsString    () const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< DEC_PathSection* >       T_PathSectionVector; 
    typedef T_PathSectionVector::const_iterator CIT_PathSectionVector;
    //@}

private:
    T_PathSectionVector pathSections_;
    uint                nNbrRefs_;              // nb of references on path
    uint                nNbrDIARefs_;           // nb of references by DIA on path
    E_State             nState_;            
    bool                bJobCanceled_;
};

#include "DEC_Path_ABC.inl"

#endif // __DEC_Path_ABC_h_