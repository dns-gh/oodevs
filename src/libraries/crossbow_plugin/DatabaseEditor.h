// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __plugins_crossbow_DatabaseEditor_h_
#define __plugins_crossbow_DatabaseEditor_h_

#include "DatabaseEditor_ABC.h"

class OGRLayer;

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  DatabaseEditor
    @brief  DatabaseEditor
*/
// Created: JCR 2009-04-22
// =============================================================================
class DatabaseEditor : public DatabaseEditor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DatabaseEditor( OGRLayer& layer );
    virtual ~DatabaseEditor();
    //@}

    //! @name
    //@{
    bool InTransaction() const;
    void BeginTransaction();
    void EndTransaction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseEditor( const DatabaseEditor& );            //!< Copy constructor
    DatabaseEditor& operator=( const DatabaseEditor& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    OGRLayer&  layer_;
    bool       inTransaction_;
    bool       useTransaction_;
    //@}
};

}
}

#endif // __plugin_crossbow_DatabaseEditor_h_
