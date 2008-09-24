// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ScopeEditor_h_
#define __crossbow_ScopeEditor_h_

namespace plugins
{
namespace crossbow
{
    class Database_ABC;

// =============================================================================
/** @class  ScopeEditor
*/
// Created: JCR 2007-12-06
// =============================================================================
class ScopeEditor
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit ScopeEditor( Database_ABC& database );
            ~ScopeEditor();
    //@}
          
private:
    //! @name Copy/Assignement
    //@{
    ScopeEditor( const ScopeEditor& );            //!< Copy constructor
    ScopeEditor& operator=( const ScopeEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC& database_;
    //@}
};

}
}

#endif // __crossbow_ScopeEditor_h_
