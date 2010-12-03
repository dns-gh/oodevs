// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitant_ABC_h_
#define __Inhabitant_ABC_h_

#include "Entity_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class InhabitantType;

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

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;

    virtual void Draw( const kernel::GlTools_ABC& tools ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant_ABC( const Inhabitant_ABC& );            //!< Copy constructor
    Inhabitant_ABC& operator=( const Inhabitant_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __Inhabitant_ABC_h_
