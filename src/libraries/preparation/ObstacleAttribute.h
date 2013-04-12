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
#include "clients_kernel/Units.h"
#include "EnumTypes.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ObstacleAttribute : public kernel::ObstacleAttribute_ABC
                        , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObstacleAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             ObstacleAttribute( gui::PropertiesDictionary& dictionary, Enum_DemolitionTargetType type, const kernel::Entity_ABC& entity );
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
    virtual bool IsReservedObstacle() const;
    virtual bool IsObstacleActivated() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObstacleAttribute( const ObstacleAttribute& );            //!< Copy constructor
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( const kernel::Entity_ABC& entity);
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesDictionary& dictionary_;
    Enum_DemolitionTargetType     type_;
    bool                          bActivated_;
    QTime                         activationTime_;
    QTime                         activityTime_;
    //@}
};

#endif // __ObstacleAttribute_h_
