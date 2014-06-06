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

#include "ADN_WorkspaceElement_ABC.h"
#include "ADN_Data_ABC.h"
#include "ADN_GUI_ABC.h"

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
        , data_( new DATA() )
        , gui_( new GUI( *data_ ) )
    {}
    //@}

    //! @name Operations
    //@{
    DATA& GetData() { return *data_; }
    GUI& GetGui() { return *gui_; }
    const DATA& GetData() const { return *data_; }
    const GUI& GetGui() const { return *gui_; }

    ADN_Data_ABC& GetDataABC() { return ( ADN_Data_ABC& ) *data_ ; }
    ADN_GUI_ABC& GetGuiABC() { return ( ADN_GUI_ABC& ) *gui_ ; }
    const ADN_Data_ABC& GetDataABC() const { return ( const ADN_Data_ABC& ) *data_ ; }
    const ADN_GUI_ABC& GetGuiABC() const { return ( const ADN_GUI_ABC& ) *gui_ ; }
    //@}

private:
    std::unique_ptr< DATA > data_;
    std::unique_ptr< GUI > gui_;
};

#endif // __ADN_WorkspaceElement_h_
