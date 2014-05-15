// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationProxy_h_
#define __LocationProxy_h_

#include "Location_ABC.h"
#include <boost/shared_ptr.hpp>

namespace kernel
{

// =============================================================================
/** @class  LocationProxy
    @brief  LocationProxy
*/
// Created: SBO 2008-06-02
// =============================================================================
class LocationProxy : public Location_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LocationProxy();
    virtual ~LocationProxy();
    //@}

    //! @name Proxy operations
    //@{
    void SetLocation( Location_ABC& location );
    void SetLocation( std::auto_ptr< Location_ABC >& location );
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    virtual void Update();
    virtual Location_ABC& Clone() const;
    virtual std::string GetTypeName() const;
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( LocationVisitor_ABC& ) const;
    virtual QString GetName() const;
    virtual bool IsValid() const;
    virtual bool IsDone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationProxy( const LocationProxy& );            //!< Copy constructor
    LocationProxy& operator=( const LocationProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< Location_ABC > location_;
    //@}
};

}

#endif // __LocationProxy_h_
