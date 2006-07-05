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

#include "ObjectPrototypeAttributes_ABC.h"

class QSpinBox;
class QCheckBox;

struct ASN1T_AttrObjectSiteFranchissement;

// =============================================================================
/** @class  CrossingSitePrototype
    @brief  CrossingSitePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSitePrototype( QWidget* parent );
    virtual ~CrossingSitePrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg );
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
    QSpinBox* width_;
    QSpinBox* depth_;
    QSpinBox* speed_;
    QCheckBox* needsConstruction_;
    ASN1T_AttrObjectSiteFranchissement* attr_;
    //@}
};

#endif // __CrossingSitePrototype_h_
