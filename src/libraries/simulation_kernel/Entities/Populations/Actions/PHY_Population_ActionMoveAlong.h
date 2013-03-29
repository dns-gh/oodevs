#ifndef __PHY_Population_ActionMoveAlong_h_
#define __PHY_Population_ActionMoveAlong_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_Population;

class PHY_Population_ActionMoveAlong : public PHY_Action_ABC
{
public:
    typedef MIL_Population ActorType;

public:
             PHY_Population_ActionMoveAlong( MIL_Population& population, const std::vector< boost::shared_ptr< MT_Vector2D > >& destination );
    virtual ~PHY_Population_ActionMoveAlong();

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    //@}

private:
    MIL_Population& population_;
    std::vector< boost::shared_ptr< MT_Vector2D > > destination_;
};

#endif // __PHY_Population_ActionMoveAlong_h_
