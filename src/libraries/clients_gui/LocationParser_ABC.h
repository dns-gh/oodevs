// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationParser_ABC_h_
#define __LocationParser_ABC_h_

namespace gui
{

// =============================================================================
/** @class  LocationParser_ABC
    @brief  Location parser interface
*/
// Created: AGE 2008-05-29
// =============================================================================
class LocationParser_ABC
{
public:

    //! @name Constructors/Destructor
    //@{
             LocationParser_ABC() {}
    virtual ~LocationParser_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const = 0;
    virtual int GetNumberOfParameters() const = 0;
    //@}
};

}

#endif // __LocationParser_ABC_h_
