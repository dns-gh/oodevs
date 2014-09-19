// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_TacticalLine_ABC_h_
#define __kernel_TacticalLine_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class ActionController;
// =============================================================================
/** @class  TacticalLine_ABC
    @brief  TacticalLine_ABC
*/
// Created: SBO 2006-10-12
// =============================================================================
class TacticalLine_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC();
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsLimit() const = 0;
    virtual void NotifyDestruction() = 0;
    virtual const std::string& GetTypeName() const;
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point, QObject* emitter ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignment operator
    //@}

};

}

#endif // __kernel_TacticalLine_ABC_h_
