// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueEditor_h_
#define __ValueEditor_h_

namespace kernel
{

template< typename T >
class ValueEditor
{
public:
             ValueEditor() {}
    virtual ~ValueEditor() {}

    virtual T GetValue() = 0;
};

}

#endif // __ValueEditor_h_
