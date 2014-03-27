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
#include "clients_kernel/ModelLoaded.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
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
    typedef std::function< geometry::Point2f( const std::string& ) > T_Converter;
    typedef std::function< std::string( const geometry::Point2f& ) > T_StringConverter;

public:
    //! @name Constructors/Destructor
    //@{
             UtmParser( kernel::Controllers& controllers, const T_Converter& converter,
                        const T_StringConverter& stringConverter );
    virtual ~UtmParser();
    //@}

    //! @name Operations
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool small ) const;
    virtual QStringList Split( const QString& input ) const;
    virtual std::string GetStringPosition( const geometry::Point2f& position ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const T_Converter converter_;
    const T_StringConverter stringConverter_;
    std::string zone_;
    //@}
};

}

#endif // __UtmParser_h_
