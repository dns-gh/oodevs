// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MgrsParser_h_
#define __MgrsParser_h_

#include "LocationParser_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
}

namespace gui
{
// =============================================================================
/** @class  MgrsParser
    @brief  MgrsParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class MgrsParser : public LocationParser_ABC
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    typedef std::function< geometry::Point2f( const std::string& ) > T_Converter;
    typedef std::function< std::string( const geometry::Point2f& ) > T_StringConverter;

public:
    //! @name Constructors/Destructor
    //@{
             MgrsParser( kernel::Controllers& controllers, const T_Converter& converter,
                         const T_StringConverter& stringConverter );
    virtual ~MgrsParser();
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

#endif // __MgrsParser_h_
