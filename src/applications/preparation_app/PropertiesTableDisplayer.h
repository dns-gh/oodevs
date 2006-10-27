// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesTableDisplayer_h_
#define __PropertiesTableDisplayer_h_

#include "clients_gui/TableItemDisplayer.h"

class KnowledgeGroupType;

// =============================================================================
/** @class  PropertiesTableDisplayer
    @brief  PropertiesTableDisplayer
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertiesTableDisplayer : public gui::TableItemDisplayer
                               , public kernel::Caller< KnowledgeGroupType >
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesTableDisplayer();
    virtual ~PropertiesTableDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Call( const KnowledgeGroupType& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertiesTableDisplayer( const PropertiesTableDisplayer& );            //!< Copy constructor
    PropertiesTableDisplayer& operator=( const PropertiesTableDisplayer& ); //!< Assignement operator
    //@}
};

#endif // __PropertiesTableDisplayer_h_
