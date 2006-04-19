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

class DisplayBuilder;

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
             CrossingSitePrototype( DisplayBuilder& builder );
    virtual ~CrossingSitePrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg ) const;
    virtual void Show() const;
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSitePrototype( const CrossingSitePrototype& );            //!< Copy constructor
    CrossingSitePrototype& operator=( const CrossingSitePrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    DisplayBuilder& display_;
//    OptionalValue< unsigned int > width_;
//    OptionalValue< unsigned int > depth_;
//    OptionalValue< unsigned int > speed_;
//    OptionalValue< bool         > needsConstruction_;
    //@}
};

#endif // __CrossingSitePrototype_h_
