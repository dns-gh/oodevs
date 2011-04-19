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

namespace sword
{
    class Path;
}

class DEC_PathPoint;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_PathResult : public DEC_Path_ABC
{
public:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< DEC_PathPoint > > T_PathPointList;
    typedef T_PathPointList::iterator                      IT_PathPointList;
    typedef T_PathPointList::const_iterator               CIT_PathPointList;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathResult();
    virtual ~DEC_PathResult();
    //@}

    //! @name Accessors
    //@{
    const T_PathPointList& GetResult( bool useAssert = true ) const;
    //@}

    //! @name Tools
    //@{
            CIT_PathPointList GetCurrentKeyOnPath         ( const MT_Vector2D& vPos ) const;
            MT_Vector2D       GetPointOnPathCloseTo       ( const MT_Vector2D& posToTest ) const;
            MT_Vector2D       GetFuturePosition           ( const MT_Vector2D& vStartPos, double rDist, bool bBoundOnPath ) const;
            bool              ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const;
    virtual void              InsertDecPoints             () = 0;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::Path& asn ) const;
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
    //@}

private:
    //! @name Member data
    //@{
    bool bSectionJustEnded_;
    //@}
};

#include "DEC_PathResult.inl"

#endif // __DEC_PathResult_h_