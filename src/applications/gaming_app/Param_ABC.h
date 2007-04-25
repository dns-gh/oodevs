// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class ActionController;
    class GlTools_ABC;
    class Viewport_ABC;
}

class Action_ABC;
struct ASN1T_MissionParameter;
struct ASN1T_OrderContext;

// =============================================================================
/** @class  Param_ABC
    @brief  Param_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class Param_ABC : public kernel::Observer_ABC
                , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Param_ABC( const QString& name );
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent ) = 0;
    virtual bool CheckValidity();
    virtual void Show() {}
    virtual void Hide() {}

    virtual void CommitTo( ASN1T_MissionParameter& ) const {};
    virtual void Clean( ASN1T_MissionParameter& ) const {};
    virtual void CommitTo( ASN1T_OrderContext& ) const {};
    virtual void Clean( ASN1T_OrderContext& ) const {};
    virtual void CommitTo( Action_ABC& ) const {};
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void InterfaceNotInitialized() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Param_ABC( const Param_ABC& );
    Param_ABC& operator=( const Param_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    kernel::ActionController* controller_;
    //@}
};

#endif // __Param_ABC_h_
