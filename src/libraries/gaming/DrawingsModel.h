// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingsModel_h_
#define __DrawingsModel_h_

#include "clients_gui/DrawerModel.h"
#include "game_asn/Messenger.h"

class DrawingFactory;

// =============================================================================
/** @class  DrawingsModel
    @brief  DrawingsModel
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingsModel : public gui::DrawerModel
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawingsModel( kernel::Controllers& controllers, const DrawingFactory& factory );
    virtual ~DrawingsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const ASN1T_MsgShapeCreation& asn );
    void Update( const ASN1T_MsgShapeUpdate& asn );
    void Delete( const ASN1T_MsgShapeDestruction& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingsModel( const DrawingsModel& );            //!< Copy constructor
    DrawingsModel& operator=( const DrawingsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const DrawingFactory& factory_;
    //@}
};

#endif // __DrawingsModel_h_
