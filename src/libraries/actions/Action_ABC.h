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

#include "game_asn/Simulation.h"
#include "clients_kernel/Entity_ABC.h"
#include "ParameterContainer_ABC.h"

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
             Action_ABC( kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target );
             Action_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target );
    virtual ~Action_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual unsigned long GetId() const;
    virtual QString GetName() const;

    virtual const kernel::Entity_ABC& GetEntity() const;
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
    //@}

protected:
    //! @name Helpers
    //@{
    void CommitTo( ASN1T_MissionParameters& asn ) const;
    void Clean( ASN1T_MissionParameters& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Action_ABC( const Action_ABC& );            //!< Copy constructor
    Action_ABC& operator=( const Action_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::OrderType& type_;
    const kernel::Entity_ABC& target_;
    unsigned long id_;
    QString name_;

    static unsigned long idManager_;
    //@}
};

}

#endif // __Action_ABC_h_
