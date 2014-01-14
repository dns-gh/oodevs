// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPath_h_
#define __ParamPath_h_

#include "ParamLocation_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
}

namespace actions
{
    namespace parameters
    {
        class Path;
    }

    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public ParamLocation_ABC< actions::parameters::Path >
                , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamPath();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void Visit( const actions::parameters::Path& param );
    virtual void FixOrigin( bool fix ) const;
    //@}

protected:
    //! @name Param_ABC
    //@{
    virtual void OnMenuClick();
    //@}

private:
    //! @name tools::ElementObserver_ABC< kernel::Entity_ABC > implementation
    //@{
    virtual void NotifyDeleted( const kernel::Entity_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* entity_;
    //@}
};

    }
}

#endif // __ParamPath_h_
