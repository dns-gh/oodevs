// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BaseDisplayer_h_
#define __BaseDisplayer_h_

#include "Displayer_ABC.h"
class Agent_ABC;
class Object;
class Team;
class DotationType;
class EquipmentType;
class Population;
class PopulationConcentration;
class CoordinateConverter_ABC;
class NBCAgent;
class ComponentType;
class BreakdownType;
class ObjectType;
class Experience;
class Morale;
class Tiredness;
class Mission;

// =============================================================================
/** @class  BaseDisplayer
    @brief  BaseDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class BaseDisplayer : public Displayer_ABC
                  , public Caller< bool >
                  , public Caller< Population >
                  , public Caller< PopulationConcentration >
                  , public Caller< Agent_ABC >
                  , public Caller< Object >
                  , public Caller< Team >
                  , public Caller< DotationType >
                  , public Caller< EquipmentType >
                  , public Caller< NBCAgent >
                  , public Caller< ComponentType >
                  , public Caller< BreakdownType >
                  , public Caller< ObjectType >
                  , public Caller< Experience >
                  , public Caller< Morale >
                  , public Caller< Tiredness >
                  , public Caller< QTime >
                  , public Caller< Mission >
{

public:
    //! @name Constructors/Destructor
    //@{
             BaseDisplayer();
    virtual ~BaseDisplayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    BaseDisplayer( const BaseDisplayer& );            //!< Copy constructor
    BaseDisplayer& operator=( const BaseDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const bool& value );
    virtual void Call( const Agent_ABC& value );
    virtual void Call( const Population& value );
    virtual void Call( const PopulationConcentration& value );
    virtual void Call( const Object& value );
    virtual void Call( const Team& value );
    virtual void Call( const DotationType& value );
    virtual void Call( const EquipmentType& value );
    virtual void Call( const NBCAgent& value );
    virtual void Call( const ComponentType& value );
    virtual void Call( const BreakdownType& value );
    virtual void Call( const ObjectType& value );
    virtual void Call( const Experience& value );
    virtual void Call( const Morale& value );
    virtual void Call( const Tiredness& value );
    virtual void Call( const QTime& value );
    virtual void Call( const Mission& value );
    //@}
};

#endif // __BaseDisplayer_h_
