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
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

class TerrainData;
class MT_Vector2D;
class TER_Pathfinder_ABC;

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
class Path_ABC : private boost::noncopyable
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
    virtual void Execute( TER_Pathfinder_ABC& pathfind );
    void Cancel();

    double GetLength() const;

    virtual bool NeedRefine() const = 0;
    virtual bool UseStrictClosest() const = 0;
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint ) = 0;
    //@}

    //! @name Accessors
    //@{
    std::size_t GetID() const;
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
    explicit Path_ABC( std::size_t identifier );
    virtual ~Path_ABC();
    //@}

    //! @name Tools
    //@{
    void RegisterPathSection( boost::shared_ptr< PathSection_ABC > section );
    std::string GetStateAsString() const;
    std::string GetPathAsString() const;
    const T_PointVector& GetComputedWaypoints() const;
    virtual void NotifySectionEnded() = 0;
    void DoExecute( TER_Pathfinder_ABC& pathfind );
    void RemoveComputedWaypoint();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< PathSection_ABC > > T_PathSectionVector;
    //@}

private:
    //! @name Member data
    //@{
    const std::size_t identifier_;
    T_PathSectionVector pathSections_;
    T_PointVector computedWaypoints_;
    E_State nState_;
    bool bJobCanceled_;
    MT_Vector2D lastWaypoint_;
    unsigned int nNbrRefs_;              // nb of references on path
    //@}
};

}
}

#endif // movement_module_Path_ABC_h_
