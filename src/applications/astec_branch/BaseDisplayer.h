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
class Agent;
class Object_ABC;
class Team;
class DotationType;
class EquipmentType;
class Population;
class PopulationConcentration;
class MT_Vector2D;

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
                  , public Caller< Agent >
                  , public Caller< Object_ABC >
                  , public Caller< Team >
                  , public Caller< DotationType >
                  , public Caller< EquipmentType >
                  , public Caller< MT_Vector2D >
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
    virtual void Call( const Agent& value );
    virtual void Call( const Population& value );
    virtual void Call( const PopulationConcentration& value );
    virtual void Call( const Object_ABC& value );
    virtual void Call( const Team& value );
    virtual void Call( const DotationType& value );
    virtual void Call( const EquipmentType& value );
    virtual void Call( const MT_Vector2D& value );
    //@}
};

#endif // __BaseDisplayer_h_
