// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Filter_ABC_h_
#define __Filter_ABC_h_

#include <boost/noncopyable.hpp>
#include <qobject.h>

// =============================================================================
/** @class  Filter_ABC
    @brief  Filter_ABC
*/
// Created: ABR 2011-06-17
// =============================================================================
class Filter_ABC : public QObject
                 , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Filter_ABC() {}
    virtual ~Filter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Execute() = 0;
    virtual const std::string GetName() const = 0;
    virtual const std::string GetDescription() const = 0;
    virtual QWidget* CreateParametersWidget( QWidget* parent ) = 0;
    virtual bool IsValid() const = 0;
    virtual bool NeedToReloadExercise() const = 0;
    //@}

signals:
    //! @name Signals
    //@{
    void statusChanged( bool valid );
    //@}
};

#endif // __Filter_ABC_h_
