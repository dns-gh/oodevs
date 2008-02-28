// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDateTime_h_
#define __ParamDateTime_h_

#include "game_asn/Simulation.h"
#include "Param_ABC.h"

class Simulation;
class ActionParameter_ABC;

// =============================================================================
/** @class  ParamDateTime
    @brief  ParamDateTime
*/
// Created: SBO 2007-05-14
// =============================================================================
class ParamDateTime : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamDateTime( QObject* parent, const QString& name, const Simulation& simulation, bool optional );
    virtual ~ParamDateTime();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( const QDateTime& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamDateTime( const ParamDateTime& );            //!< Copy constructor
    ParamDateTime& operator=( const ParamDateTime& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QDateTime date_;
    bool optional_;
    //@}
};

#endif // __ParamDateTime_h_
