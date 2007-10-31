// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligence_ABC_h_
#define __Intelligence_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class Karma;
    class HierarchyLevel_ABC;

// =============================================================================
/** @class  Intelligence_ABC
    @brief  Intelligence_ABC
*/
// Created: SBO 2007-10-12
// =============================================================================
class Intelligence_ABC : public kernel::Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligence_ABC();
    virtual ~Intelligence_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetSymbol() const = 0;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const = 0;
    virtual const kernel::Karma& GetKarma() const = 0;
    virtual bool IsEmbarked() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Delete() = 0;
    virtual void Rename( const QString& name ) = 0;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence_ABC( const Intelligence_ABC& );            //!< Copy constructor
    Intelligence_ABC& operator=( const Intelligence_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __Intelligence_ABC_h_
