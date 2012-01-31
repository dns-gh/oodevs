//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.h $
// $Author: Age $
// $Modtime: 28/02/05 10:04 $
// $Revision: 2 $
// $Workfile: DEC_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Path_h_
#define __DEC_Path_h_

#include "DEC_Path_ABC.h"
#include <string>
#include <vector>

class DEC_PathSection_ABC;
class TerrainData;
class MT_Vector2D;

// =============================================================================
// @class  DEC_Path
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Path : public DEC_Path_ABC
{
public:
    //! @name Operations
    //@{
    virtual void Execute( TerrainPathfinder& pathfind );
    virtual void CleanAfterComputation();
    void Cancel();

    virtual double GetLength() const;
    virtual bool NeedRefine() const = 0;
    virtual bool UseStrictClosest() const = 0;

    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual E_State GetState() const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             DEC_Path();
    virtual ~DEC_Path();
    //@}

    //! @name Tools
    //@{
    void RegisterPathSection( DEC_PathSection_ABC& section );

    std::string GetStateAsString() const;
    std::string GetPathAsString() const;

    virtual void NotifySectionEnded() = 0;

    void DoExecute( TerrainPathfinder& pathfind );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< DEC_PathSection_ABC* >   T_PathSectionVector;
    typedef T_PathSectionVector::const_iterator CIT_PathSectionVector;
    //@}

private:
    T_PathSectionVector pathSections_;
    E_State nState_;
    bool bJobCanceled_;
};

#include "DEC_Path.inl"

#endif // __DEC_Path_h_
