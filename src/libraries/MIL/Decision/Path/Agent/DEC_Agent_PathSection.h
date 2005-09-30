//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_PathSection.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: DEC_Agent_PathSection.h $
//
//*****************************************************************************

#ifndef __DEC_Agent_PathSection_h_
#define __DEC_Agent_PathSection_h_

#include "MIL.h"

#include "Decision/Path/DEC_PathSection_ABC.h"

class DEC_Agent_Path;
class TerrainRule_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Agent_PathSection : public DEC_PathSection_ABC
{
    MT_COPYNOTALLOWED( DEC_Agent_PathSection );

public:
             DEC_Agent_PathSection( DEC_Agent_Path& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );
    virtual ~DEC_Agent_PathSection();

private:
    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule   () const;
    virtual bool             NeedRefine() const;
    //@}

private:
    TerrainRule_ABC& rule_;
    bool             bNeedRefine_;
};

#include "DEC_Agent_PathSection.inl"

#endif // __DEC_Agent_PathSection_h_