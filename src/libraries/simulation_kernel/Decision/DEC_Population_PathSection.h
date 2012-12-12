//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_PathSection.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: DEC_Population_PathSection.h $
//
//*****************************************************************************

#ifndef __DEC_Population_PathSection_h_
#define __DEC_Population_PathSection_h_

#include "Decision/DEC_PathSection_ABC.h"

class DEC_Population_Path;
class DEC_Population_PathfinderRule;
class TerrainRule_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Population_PathSection : public DEC_PathSection_ABC
{
public:
             DEC_Population_PathSection( DEC_Population_Path& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );
    virtual ~DEC_Population_PathSection();

private:
    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule() const;
    //@}

private:
    std::auto_ptr< DEC_Population_PathfinderRule > rule_;
};

#endif // __DEC_Population_PathSection_h_