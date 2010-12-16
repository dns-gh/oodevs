// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_ABC_h_
#define __ActionParameter_ABC_h_

#include "ParameterContainer_ABC.h"
#include <geometry/types.h>
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

namespace sword
{
    class MissionParameter;
    class MissionParameter_Value;
}

namespace kernel
{
    class Displayer_ABC;
    class Viewport_ABC;
    class GlTooltip_ABC;
    class GlTools_ABC;
}

namespace xml
{
    class xostream;
}

namespace actions
{
    class ParameterVisitor_ABC;

// =============================================================================
/** @class  Parameter_ABC
    @brief  Parameter_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class Parameter_ABC : public ParameterContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Parameter_ABC( const QString& name );
    virtual ~Parameter_ABC();
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
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual void Serialize( xml::xostream& xos ) const;

    virtual void AddParameter( Parameter_ABC& parameter );
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;

    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void Set( bool isSet );
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void DrawToolTip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Parameter_ABC( const Parameter_ABC& );            //!< Copy constructor
    Parameter_ABC& operator=( const Parameter_ABC& ); //!< Assignment operator
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

}

#endif // __ActionParameter_ABC_h_
