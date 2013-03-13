// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UtmParser_h_
#define __UtmParser_h_

#include "LocationParser_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  UtmParser
    @brief  UtmParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class UtmParser : public LocationParser_ABC
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UtmParser( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UtmParser();
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const;
    virtual int GetNumberOfParameters() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    static QString Fill( QString value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    std::string zone_;
    //@}
};

}

#endif // __UtmParser_h_
