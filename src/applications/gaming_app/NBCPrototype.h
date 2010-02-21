// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_h_
#define __NBCPrototype_h_

#include "clients_gui/NBCPrototype_ABC.h"

namespace MsgsClientToSim 
{
    class MsgMagicActionCreateObject;
}

struct MsgObjectAttributesNbcZone;
struct MsgObjectAttributesNbcCloud;

// =============================================================================
/** @class  NBCPrototype
    @brief  NBCPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype : public gui::NBCPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    NBCPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, int maxToxic, MsgsClientToSim::MsgMagicActionCreateObject& msg );
    virtual ~NBCPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCPrototype( const NBCPrototype& );            //!< Copy constructor
    NBCPrototype& operator=( const NBCPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    MsgsClientToSim::MsgMagicActionCreateObject& msg_;
    //@}
};

#endif // __NBCPrototype_h_
