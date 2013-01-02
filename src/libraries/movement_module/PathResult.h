// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef movement_module_PathResult_h_
#define movement_module_PathResult_h_

#include "Path_ABC.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#include <boost/shared_ptr.hpp>

class DEC_Knowledge_Object;
class KnowledgeCache;

namespace sword
{
    class Path;

namespace wrapper
{
    class View;
}

namespace movement
{
    class PathPoint;
    class PathType;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class PathResult : public Path_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectVector;
    typedef T_KnowledgeObjectVector::const_iterator                CIT_KnowledgeObjectVector;

    typedef std::list< boost::shared_ptr< PathPoint > >          T_PathPointList;
    typedef T_PathPointList::iterator                                IT_PathPointList;
    typedef T_PathPointList::const_iterator                          CIT_PathPointList;
    typedef std::list< std::pair< MT_Vector2D, CIT_PathPointList > > T_FollowingPathList;
    typedef T_FollowingPathList::const_iterator                      CIT_FollowingPathList;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PathResult( const PathType& pathType );
    virtual ~PathResult();
    //@}

    //! @name Accessors
    //@{
    const T_PathPointList& GetResult( bool useCheck = true ) const;
    const PathType& GetPathType() const;
    //@}

    //! @name Tools
    //@{
    CIT_PathPointList GetCurrentKeyOnPath() const;
    MT_Vector2D GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const;
    bool ComputeFutureObjectCollision( const wrapper::View& entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, bool blockedByObject, bool applyScale ) const;
    virtual void InsertDecPoints() = 0;
    virtual void NotifyPointReached( const CIT_PathPointList& itCurrentPathPoint );
    virtual bool IsWaypoint( const MT_Vector2D& point ) const;
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySectionEnded();
    MT_Vector2D InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, double rDist, bool bBoundOnPath ) const;
    //@}

protected:
    //! @name Member data
    //@{
    T_PathPointList resultList_; //$$$
    CIT_PathPointList itCurrentPathPoint_;
    //@}

private:
    //! @name Member data
    //@{
    const PathType& pathType_;
    bool bSectionJustEnded_;
    //@}
};

}
}

#endif // movement_module_PathResult_h_
