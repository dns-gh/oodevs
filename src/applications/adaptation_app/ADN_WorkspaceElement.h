// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_WorkspaceElement_h_
#define __ADN_WorkspaceElement_h_

#include "ADN_Data_ABC.h"
#include "ADN_GUI_ABC.h"
#include "ADN_WorkspaceElement_ABC.h"

// =============================================================================
/** @class  ADN_WorkspaceElement
    @brief  ADN_WorkspaceElement
*/
// Created: APE 2004-12-06
// =============================================================================
template< class DATA, class GUI >
class ADN_WorkspaceElement : public ADN_WorkspaceElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_WorkspaceElement( E_WorkspaceElements element )
                 : ADN_WorkspaceElement_ABC( ADN_Tr::ConvertFromWorkspaceElement( element ).c_str() )
                 , data_( *new DATA() )
                 , gui_( *new GUI( data_ ) )
    {}
    virtual ~ADN_WorkspaceElement()
    {
        delete &(ADN_Data_ABC&)data_;
        delete &(ADN_GUI_ABC&)gui_;
    }
    //@}

    //! @name Accessors
    //@{
    DATA& GetData() { return data_; }
    GUI&  GetGui()  { return gui_; }

    ADN_Data_ABC& GetDataABC() { return (ADN_Data_ABC&)data_; }
    ADN_GUI_ABC&  GetGuiABC()  { return (ADN_GUI_ABC&)gui_; }
    //@}

private:
    DATA& data_;
    GUI&  gui_;
};

#endif // __ADN_WorkspaceElement_h_
