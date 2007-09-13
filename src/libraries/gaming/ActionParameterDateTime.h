// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDateTime_h_
#define __ActionParameterDateTime_h_

#include "ActionParameter.h"
#include "game_asn/Asn.h"

class Simulation;

// =============================================================================
/** @class  ActionParameterDateTime
    @brief  ActionParameterDateTime
*/
// Created: SBO 2007-05-15
// =============================================================================
class ActionParameterDateTime : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterDateTime( const kernel::OrderParameter& parameter, xml::xistream& xis, const Simulation& simulation );
             ActionParameterDateTime( const kernel::OrderParameter& parameter, const Simulation& simulation, unsigned int ticks );
    virtual ~ActionParameterDateTime();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    void CommitTo( ASN1INT& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterDateTime( const ActionParameterDateTime& );            //!< Copy constructor
    ActionParameterDateTime& operator=( const ActionParameterDateTime& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Simulation& simulation_;
    unsigned int day_;
    QTime time_;
    //@}
};

#endif // __ActionParameterDateTime_h_
