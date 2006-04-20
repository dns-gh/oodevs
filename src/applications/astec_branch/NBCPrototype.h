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

#include "ObjectPrototypeAttributes_ABC.h"
#include "Resolver.h"
#include "ValuedComboBox.h"

class NBCAgent;

struct ASN1T_AttrObjectZoneNBC;
struct ASN1T_AttrObjectNuageNBC;

// =============================================================================
/** @class  NBCPrototype
    @brief  NBCPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver );
    virtual ~NBCPrototype();
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
    NBCPrototype( const NBCPrototype& );            //!< Copy constructor
    NBCPrototype& operator=( const NBCPrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ValuedComboBox< const NBCAgent* >* nbcAgents_;
    ASN1T_AttrObjectZoneNBC* nbcArea_;
    ASN1T_AttrObjectNuageNBC* nbcCloud_;
    //@}
};

#endif // __NBCPrototype_h_
