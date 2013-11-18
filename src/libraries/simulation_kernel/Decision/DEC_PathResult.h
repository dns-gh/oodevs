// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_PathResult_h_
#define __DEC_PathResult_h_

#include "Decision/DEC_Path.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "MT_Tools/Mt_Vector2DTypes.h"
#include "MT_Tools/MT_Logger.h"

namespace sword
{
    class Path;
}

class MIL_Agent_ABC;
class DEC_PathPoint;
class DEC_PathType;
class MIL_Object_ABC;
class PHY_RawVisionData;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_PathResult : public DEC_Path
{
public:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< DEC_PathPoint > >          T_PathPointList;
    typedef T_PathPointList::iterator                                IT_PathPointList;
    typedef T_PathPointList::const_iterator                          CIT_PathPointList;
    typedef std::list< std::pair< MT_Vector2D, CIT_PathPointList > > T_FollowingPathList;
    typedef T_FollowingPathList::const_iterator                      CIT_FollowingPathList;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_PathResult( const DEC_PathType& pathType );
    virtual ~DEC_PathResult();
    //@}

    //! @name Accessors
    //@{
    const T_PathPointList& GetResult( bool useCheck = true ) const;
    const DEC_PathType& GetPathType() const;
    //@}

    //! @name Tools
    //@{
    bool IsOnPath( const MT_Vector2D& vPos ) const;
    CIT_PathPointList GetCurrentKeyOnPath() const;
    MT_Vector2D GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const;
    bool ComputeFutureObjectCollision( const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const;
    virtual void InsertDecPoints() = 0;
    virtual void NotifyPointReached( const CIT_PathPointList& itCurrentPathPoint );
    virtual bool IsWaypoint( const MT_Vector2D& point ) const;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::Path& asn, int firstPoint, int pathSizeThreshold ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySectionEnded();
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
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
    const DEC_PathType& pathType_;
    bool bSectionJustEnded_;
    const PHY_RawVisionData& elevation_;
    //@}
};

#include "DEC_PathResult.inl"

#endif // __DEC_PathResult_h_