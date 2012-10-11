//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/Path_ABC.h $
// $Author: Age $
// $Modtime: 28/02/05 10:04 $
// $Revision: 2 $
// $Workfile: Path_ABC.h $
//
//*****************************************************************************

#ifndef movement_module_Path_ABC_h_
#define movement_module_Path_ABC_h_

#include "MT_Tools/Mt_Vector2DTypes.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <vector>

class TerrainData;
class TerrainPathfinder;
class MT_Vector2D;

namespace sword
{
namespace movement
{
    class PathSection_ABC;
    class Agent_PathClass;

// =============================================================================
// @class  Path_ABC
// Created: NLD 2005-02-22
// =============================================================================
class Path_ABC : public boost::enable_shared_from_this< Path_ABC >, private boost::noncopyable
{
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
    virtual void ComputePath( const boost::shared_ptr< Path_ABC >& pPath ) = 0;
    void Cancel();
    virtual void CleanAfterComputation();

    double GetLength() const;
    void AddRef();
    void DecRef();

    virtual bool NeedRefine() const = 0;
    virtual bool UseStrictClosest() const = 0;
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint ) = 0;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    E_State GetState() const;
    const MT_Vector2D& GetLastWaypoint() const;

    virtual const Agent_PathClass& GetClass() const = 0;
    virtual const MT_Vector2D* GetFirstPoint() const = 0;
    //@}

    //! @name Operators
    //@{
    bool operator==( const Path_ABC& rhs ) const;
    bool operator!=( const Path_ABC& rhs ) const;
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             Path_ABC();
    virtual ~Path_ABC();
    //@}

    //! @name Tools
    //@{
    void RegisterPathSection( PathSection_ABC& section );
    std::string GetStateAsString() const;
    std::string GetPathAsString() const;
    const T_PointVector& GetComputedWaypoints() const;
    virtual void NotifySectionEnded() = 0;
    void DoExecute( TerrainPathfinder& pathfind );
    void RemoveComputedWaypoint();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PathSection_ABC* >       T_PathSectionVector;
    typedef T_PathSectionVector::const_iterator CIT_PathSectionVector;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int nID_;
    T_PathSectionVector pathSections_;
    T_PointVector computedWaypoints_;
    E_State nState_;
    bool bJobCanceled_;
    MT_Vector2D lastWaypoint_;
    unsigned int nNbrRefs_;              // nb of references on path
    //@}

private:
    static unsigned int nIDIdx_;
};

}
}

#endif // movement_module_Path_ABC_h_
