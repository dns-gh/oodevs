// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Report_h_
#define __Report_h_

#include <geometry/Types.h>
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtGui/qcolor.h>
#pragma warning( pop )

namespace kernel
{
    class Entity_ABC;
    class Displayer_ABC;
}


// =============================================================================
/** @class  Report
    @brief  Report
    // $$$$ AGE 2006-06-29: Refactor !
    // $$$$ AGE 2006-07-03: C'est _vraiment_ à chier
*/
// Created: APE 2004-08-04
// =============================================================================
class Report
{
public:
    enum E_Type
    {
        eRC      = 1000,
        eTrace   = 1001,
        eEvent   = 1002,
        eMessage = 1003,
        eWarning = 1004
    };

public:
    //! @name Constructors/Destructor
    //@{
             Report( const kernel::Entity_ABC& entity, E_Type type, const QString& message, const QDateTime& time );
    virtual ~Report();
    //@}

    //! @name Operations
    //@{
    void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    void Read();
    //@}

    //! @name Accessors
    //@{
    const kernel::Entity_ABC& GetOwner() const;
    E_Type GetType () const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Report( const Report& );
    Report& operator=( const Report& );
    //@}

    //! @name Helpers
    //@{
    static QColor GetColor( E_Type type );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    E_Type                    type_;
    QString                   message_;
    QDateTime                 time_;
    bool                      isNew_;
    geometry::Point2f         point_;
    //@}
};

#endif // __Report_h_
