// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MinePrototype_h_
#define __MinePrototype_h_

#include "clients_gui/MinePrototype_ABC.h"

struct ASN1T_MagicActionCreateObject;
struct ASN1T_AttrObjectBouchonMines;
struct ASN1T_AttrObjectZoneMineeLineaire;
struct ASN1T_AttrObjectZoneMineeParDispersion;

// =============================================================================
/** @class  MinePrototype
    @brief  MinePrototype
*/
// Created: SBO 2007-02-08
// =============================================================================
class MinePrototype : public gui::MinePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MinePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg );
    virtual ~MinePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MinePrototype( const MinePrototype& );            //!< Copy constructor
    MinePrototype& operator=( const MinePrototype& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_AttrObjectBouchonMines* attrMineJam_;
    ASN1T_AttrObjectZoneMineeLineaire* attrLinearMinedArea_;
    ASN1T_AttrObjectZoneMineeParDispersion* attrDispersedMinedArea_;
    //@}
};

#endif // __MinePrototype_h_
