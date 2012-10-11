//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint.h $
// $Author: Age $
// $Modtime: 3/02/05 18:16 $
// $Revision: 4 $
// $Workfile: Rep_PathPoint.h $
//
//*****************************************************************************

#ifndef movement_module_Rep_PathPoint_h_
#define movement_module_Rep_PathPoint_h_

#include "PathPoint.h"
#include "wrapper/View.h"
#include <boost/optional.hpp>

namespace sword
{
namespace movement
{
//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class Rep_PathPoint : public PathPoint
{
public:
    //! @name Constructors/Destructor
    //@{
             Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType = "Rep_Point" );
    virtual ~Rep_PathPoint();
    //@}

    //! @name Accessors
    //@{
    virtual const TerrainData& GetTypeTerrain() const;
    //@}

    //! @name DIA
    //@{
    virtual void SendToDIA( unsigned int entity, boost::shared_ptr< PathPoint > point ) const;
    virtual void RemoveFromDIA( boost::shared_ptr< PathPoint > self );
    //@}

private:
    //! @name Member data
    //@{
    TerrainData nTypeTerrain_;
    mutable boost::optional< unsigned int > entity_;
    //@}
};

}
}

#endif // movement_module_Rep_PathPoint_h_
