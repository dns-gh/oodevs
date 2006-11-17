// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesTableDisplayer_h_
#define __PropertiesTableDisplayer_h_

#include "clients_gui/TableItemDisplayer.h"
#include "preparation/EnumTypes.h"

class TeamKarma;
class DotationsItem;

// =============================================================================
/** @class  PropertiesTableDisplayer
    @brief  PropertiesTableDisplayer
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertiesTableDisplayer : public gui::TableItemDisplayer
                               , public kernel::Caller< TeamKarma >
                               , public kernel::Caller< Enum_PopulationAttitude >
                               , public kernel::Caller< DotationsItem >
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesTableDisplayer();
    virtual ~PropertiesTableDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Call( const TeamKarma& value );
    virtual void Call( const Enum_PopulationAttitude& value );
    virtual void Call( const DotationsItem& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertiesTableDisplayer( const PropertiesTableDisplayer& );            //!< Copy constructor
    PropertiesTableDisplayer& operator=( const PropertiesTableDisplayer& ); //!< Assignement operator
    //@}
};

#endif // __PropertiesTableDisplayer_h_
