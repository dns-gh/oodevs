// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_ABC_h_
#define __ActionParameter_ABC_h_

#include "game_asn/Simulation.h"
#include "ActionParameterContainer_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class GlTooltip_ABC;
}

namespace xml
{
    class xostream;
}

class ActionParameterVisitor_ABC;

// =============================================================================
/** @class  ActionParameter_ABC
    @brief  ActionParameter_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionParameter_ABC : public ActionParameterContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameter_ABC( const QString& name );
    virtual ~ActionParameter_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual std::string GetType() const;
    virtual geometry::Point2f GetPosition() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual void Serialize( xml::xostream& xos ) const;

    virtual void AddParameter( ActionParameter_ABC& parameter );
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;

    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void Set( bool isSet );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    void DrawToolTip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameter_ABC( const ActionParameter_ABC& );            //!< Copy constructor
    ActionParameter_ABC& operator=( const ActionParameter_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    geometry::Point2f position_;
    std::auto_ptr< kernel::GlTooltip_ABC > toolTip_;
    bool isSet_;
    //@}
};

#endif // __ActionParameter_ABC_h_
