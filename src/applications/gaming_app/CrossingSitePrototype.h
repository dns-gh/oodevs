// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSitePrototype_h_
#define __CrossingSitePrototype_h_

#include "clients_gui/CrossingSitePrototype_ABC.h"

namespace MsgsClientToSim
{
    class MsgMagicActionCreateObject;
}

// =============================================================================
/** @class  CrossingSitePrototype
    @brief  CrossingSitePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype : public gui::CrossingSitePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    CrossingSitePrototype( QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& msg );
    virtual ~CrossingSitePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSitePrototype( const CrossingSitePrototype& );            //!< Copy constructor
    CrossingSitePrototype& operator=( const CrossingSitePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    MsgsClientToSim::MsgMagicActionCreateObject& msg_;
    //@}
};

#endif // __CrossingSitePrototype_h_
