// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitant_ABC_h_
#define __Inhabitant_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class InhabitantType;
    class AutomatType;

// =============================================================================
/** @class  Inhabitant_ABC
    @brief  Inhabitant_ABC
*/
// Created: SLG 2010-11-23
// =============================================================================
class Inhabitant_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant_ABC();
    virtual ~Inhabitant_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual const kernel::InhabitantType& GetType() const = 0;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    unsigned int GetHealthy() const { return healthy_; }
    unsigned int GetWounded() const { return wounded_; }
    unsigned int GetDead() const { return dead_; }
    //@}

protected:
    unsigned int healthy_;
    unsigned int wounded_;
    unsigned int dead_;

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant_ABC( const Inhabitant_ABC& );            //!< Copy constructor
    Inhabitant_ABC& operator=( const Inhabitant_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __Inhabitant_ABC_h_
