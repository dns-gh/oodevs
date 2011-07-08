// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Action_ABC_h_
#define __Action_ABC_h_

#include "clients_kernel/Entity_ABC.h"
#include "ParameterContainer_ABC.h"
#pragma warning( push, 0 )
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )
#pragma warning( pop )

namespace sword
{
    class MissionParameters;
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

enum EActionType
{
    eTypeEntity,
    eTypeObjects,
    eTypeWeather,
    eTypeMagic
};

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
    static EActionType actionTypeEntity_;
    static EActionType actionTypeObjects_;
    static EActionType actionTypeWeather_;
    static EActionType actionTypeMagic_;
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
    virtual bool IsValid() const;

    virtual const kernel::OrderType& GetType() const;
    virtual void AddParameter( Parameter_ABC& parameter );
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher, int context = 0 ) const = 0;

    virtual void Polish() = 0;
    virtual void Rename( const QString& name );

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    void RegisterAndPublish( ActionsModel& actionsModel );
    virtual void Invalidate();
    //@}

protected:
    //! @name Helpers
    //@{
    void CommitTo( sword::MissionParameters& message ) const;
    void Clean( sword::MissionParameters& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    bool CheckKnowledgeValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::OrderType& type_;
    unsigned long id_;
    QString name_;
    mutable bool valid_;
    static unsigned long idManager_;
    //@}
};

}

#endif // __Action_ABC_h_
