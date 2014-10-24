// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Tooltip_h_
#define __Tooltip_h_

namespace gui
{
// =============================================================================
/** @class  Tooltip
    @brief  Tooltip
*/
// Created: AGE 2006-06-29
// =============================================================================
class Tooltip
{
public:
    struct Message
    {
        QString message_;
        QColor color_;
        QFont font_;
    };
    typedef std::vector< Message > T_Messages;

public:
    explicit Tooltip( const T_Messages& messages );

    bool Empty() const;
    QImage GenerateImage() const;

    bool operator<( const Tooltip& rhs ) const;

private:
    T_Messages messages_;
};

}

#endif // __Tooltip_h_
