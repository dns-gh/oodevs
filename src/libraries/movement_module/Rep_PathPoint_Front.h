//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Front.h $
// $Author: Jvt $
// $Modtime: 3/12/04 11:39 $
// $Revision: 2 $
// $Workfile: Rep_PathPoint_Front.h $
//
//*****************************************************************************

#ifndef movement_module_Rep_PathPoint_Front_h_
#define movement_module_Rep_PathPoint_Front_h_

#include "PathPoint.h"
#include "wrapper/View.h"
#include <boost/optional.hpp>

namespace sword
{
namespace movement
{
    class Rep_PathPoint;

//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class Rep_PathPoint_Front  : public PathPoint
{
public:
    //! @name Constructors/Destructor
    //@{
             Rep_PathPoint_Front( const MT_Vector2D& vPos, boost::shared_ptr< PathPoint > dest );
    virtual ~Rep_PathPoint_Front();
    //@}

    //! @name DIA
    //@{
    virtual void SendToDIA( unsigned int entity, boost::shared_ptr< PathPoint > point ) const;
    virtual void RemoveFromDIA( boost::shared_ptr< PathPoint > self );
    boost::shared_ptr< PathPoint > GetDestPoint() const;
    //@}

private:
    boost::shared_ptr< PathPoint > destPoint_;
    mutable boost::optional< unsigned int > entity_;
};

}
}

#endif // movement_module_Rep_PathPoint_Front_h_
