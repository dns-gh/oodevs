//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/Agent_PathSection.h $
// $Author: Age $
// $Modtime: 10/05/05 16:48 $
// $Revision: 6 $
// $Workfile: Agent_PathSection.h $
//
//*****************************************************************************

#ifndef sword_movement_Agent_PathSection_h_
#define sword_movement_Agent_PathSection_h_

#include "PathSection_ABC.h"

class TerrainRule_ABC;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace movement
{
    class Agent_Path;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2005-02-22
// =============================================================================
class Agent_PathSection : public PathSection_ABC
{
public:
             Agent_PathSection( const wrapper::View& entity, Agent_Path& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint );
    virtual ~Agent_PathSection();

private:
    //! @name Accessors
    //@{
    virtual TerrainRule_ABC& GetRule() const;
    //@}

private:
    TerrainRule_ABC& rule_;
};

}
}

#endif // sword_movement_Agent_PathSection_h_
