// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_UrbanObject_h_
#define __kernel_UrbanObject_h_

#include "UrbanObject_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class UrbanDisplayOptions;

// =============================================================================
/** @class  UrbanObject
    @brief  Urban object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject : public UrbanObject_ABC
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< UrbanDisplayOptions >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( Controllers& controllers, const std::string& name, unsigned int id,
                          const ObjectType& type, const AccommodationTypes& accommodations,
                          UrbanDisplayOptions& options);
             UrbanObject( xml::xistream& xis, Controllers& controllers, const ObjectType& type,
                          const AccommodationTypes& accommodations, UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateColor();
    virtual void NotifyUpdated( const UrbanDisplayOptions& );
    //@}

protected:
    //! @name Member data
    //@{
    Controllers& controllers_;
    UrbanDisplayOptions& options_;
    //@}
};

} //! using namespace kernel

#endif // __kernel_UrbanObject_h_
