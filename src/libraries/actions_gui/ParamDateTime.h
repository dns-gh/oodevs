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

#include "Param_ABC.h"

namespace actions
{
    namespace gui
    {

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
             ParamDateTime( QObject* parent, const QString& name, const QDateTime& current, bool optional );
    virtual ~ParamDateTime();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
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
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    QDateTime date_;
    bool optional_;
    //@}
};

    }
}

#endif // __ParamDateTime_h_
