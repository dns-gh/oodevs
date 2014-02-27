// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ObstacleAttribute : public kernel::ObstacleAttribute_ABC
                        , public kernel::Serializable_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             ObstacleAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~ObstacleAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Activate( bool activate );
    void SetActivationTime( int time );
    void SetActivityTime( int time );
    virtual bool IsObstacleActivated() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( const kernel::Entity_ABC& entity);
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesDictionary& dictionary_;
    bool bActivated_;
    kernel::Duration activationTime_;
    kernel::Duration activityTime_;
    //@}
};

#endif // __ObstacleAttribute_h_
