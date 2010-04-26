// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Action_ABC_h_
#define __Action_ABC_h_

#include "clients_kernel/Entity_ABC.h"
#include "ParameterContainer_ABC.h"

namespace Common
{
    class MsgMissionParameters;
}

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Displayer_ABC;
    class OrderType;
    class Viewport_ABC;
    class GlTools_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class Publisher_ABC;

namespace actions
{
class ActionsModel;

// =============================================================================
/** @class  Action_ABC
    @brief  Action_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class Action_ABC : public kernel::Entity_ABC
                 , public ParameterContainer_ABC
{

public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Action_ABC( kernel::Controller& controller, const kernel::OrderType& type );
             Action_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type );
    virtual ~Action_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual unsigned long GetId() const;
    virtual QString GetName() const;

    virtual const kernel::OrderType& GetType() const;
    virtual void AddParameter( Parameter_ABC& parameter );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher ) const = 0;

    virtual void Polish() = 0;
    virtual void Rename( const QString& name );

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    void RegisterAndPublish( ActionsModel& actionsModel );
    //@}

protected:
    //! @name Helpers
    //@{
    void CommitTo( Common::MsgMissionParameters& message ) const;
    void Clean( Common::MsgMissionParameters& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::OrderType& type_;
    unsigned long id_;
    QString name_;

    static unsigned long idManager_;
    //@}
};

}

#endif // __Action_ABC_h_
