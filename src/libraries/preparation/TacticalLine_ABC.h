// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class GlTools_ABC;
    class Controller;
}

namespace xml
{
    class xistream;
    class xostream;
}

class IdManager;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public kernel::TacticalLine_ABC
                       , public kernel::Extension_ABC
                       , public kernel::Drawable_ABC
                       , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( kernel::Controller& controller, const QString& baseName, IdManager& idManager );
             TacticalLine_ABC( kernel::Controller& controller, xml::xistream& xis, IdManager& idManager );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId  () const;
    virtual QString       GetName() const;
    virtual bool          IsLimit() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );
    TacticalLine_ABC& operator=( const TacticalLine_ABC& );
    //@}

    //! @name Helpers
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    unsigned long  id_;
    QString        name_;
    //@}
};

#endif // __TacticalLine_ABC_h_
