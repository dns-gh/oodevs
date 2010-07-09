// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __XyParser_h_
#define __XyParser_h_

#include "LocationParser_ABC.h"

namespace gui
{

// =============================================================================
/** @class  XyParser
    @brief  XyParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class XyParser : public LocationParser_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             XyParser();
    virtual ~XyParser();
    //@}

    //! @name Operations
    //@{
    bool Parse( QString content, geometry::Point2f& result, QString& hint ) const;
    virtual bool Parse( QString content, geometry::Point2f& result, QStringList& hint ) const;
    virtual int GetNumberOfParameters() const;

    //@}

private:
    int numParameters_;
};

}

#endif // __XyParser_h_
