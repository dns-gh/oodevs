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

#include "Decision/DEC_Path_ABC.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "MT_Tools/Mt_Vector2DTypes.h"

namespace sword
{
    class Path;
}

class MIL_Agent_ABC;
class DEC_PathPoint;
class DEC_PathType;
class TER_Polygon;
class TER_Localisation;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_PathResult : public DEC_Path_ABC
{
public:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< DEC_PathPoint > > T_PathPoints;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_PathResult( const DEC_PathType& pathType );
    virtual ~DEC_PathResult();
    //@}

    //! @name Accessors
    //@{
    const T_PathPoints& GetResult() const;
    const DEC_PathType& GetPathType() const;
    //@}

    //! @name Tools
    //@{
    bool IsOnPath( const MT_Vector2D& vPos ) const;
    T_PathPoints::const_iterator GetCurrentKeyOnPath() const;
    MT_Vector2D GetFuturePosition( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const;
    bool ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest,
        double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject,
        const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const;
    virtual void Finalize() = 0;
    virtual void NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint );
    virtual const MT_Vector2D& GetLastWaypoint() const = 0;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::Path& asn, int firstPoint, int pathSizeThreshold ) const;
    //@}

private:
    //! @name Helpers
    //@{
    MT_Vector2D InternalGetFuturePosition( const T_PathPoints::const_iterator& itCurrentPos, double rDist, bool bBoundOnPath ) const;
    std::pair< TER_Polygon, std::size_t > ComputePathHull( const T_PathPoints::const_iterator& itCurrentPathPoint ) const;
    bool HullIntersects( const TER_Polygon& hull, const TER_Localisation& location ) const;
    void ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, double& rDistance,
        boost::shared_ptr< DEC_Knowledge_Object >& pObject, const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge,
        const T_PathPoints::const_iterator& itCurrentPathPoint, const TER_Localisation& location ) const;
    //@}

protected:
    void SetResult( T_PathPoints points );

protected:
    //! @name Member data
    //@{
    T_PathPoints resultList_; //$$$
    //@}

private:
    //! @name Member data
    //@{
    T_PathPoints::const_iterator itCurrentPathPoint_;
    const DEC_PathType& pathType_;
    bool bSectionJustStarted_;
    //@}
};

#endif // __DEC_PathResult_h_