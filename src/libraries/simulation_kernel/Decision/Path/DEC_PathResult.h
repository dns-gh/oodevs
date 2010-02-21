//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathResult.h $
// $Author: Age $
// $Modtime: 16/06/05 15:09 $
// $Revision: 10 $
// $Workfile: DEC_PathResult.h $
//
//*****************************************************************************

#ifndef __DEC_PathResult_h_
#define __DEC_PathResult_h_

#include "MIL.h"
#include "Decision/Path/DEC_Path_ABC.h"
#include "Knowledge/DEC_Knowledge_Def.h"

namespace Common
{
    class MsgPath;
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
    typedef std::list< DEC_PathPoint* >     T_PathPointList;
    typedef T_PathPointList::iterator       IT_PathPointList;
    typedef T_PathPointList::const_iterator CIT_PathPointList;    
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathResult();
    virtual ~DEC_PathResult();
    //@}
   
    //! @name Accessors 
    //@{
    const T_PathPointList& GetResult() const;
    //@}
    
    //! @name Tools
    //@{
            CIT_PathPointList GetCurrentKeyOnPath         ( const MT_Vector2D& vPos ) const;
            MT_Vector2D       GetPointOnPathCloseTo       ( const MT_Vector2D& posToTest ) const;
            MT_Vector2D       GetFuturePosition           ( const MT_Vector2D& vStartPos, MT_Float rDist, bool bBoundOnPath ) const;
            bool              ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const;
    virtual void              InsertDecPoints             () = 0;
    //@}

    //! @name Network
    //@{
    void Serialize( Common::MsgPath& asn ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySectionEnded();
    virtual void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    MT_Vector2D InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, MT_Float rDist, bool bBoundOnPath ) const;
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