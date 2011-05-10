// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PluginSetting_h_
#define __PluginSetting_h_

#include <boost/noncopyable.hpp>

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QTimeEdit;
class QWidget;

namespace xml
{
    class xistream;
}

namespace frontend
{
    class PluginSettingVisitor_ABC;

// =============================================================================
/** @class  PluginSetting
    @brief  PluginSetting
*/
// Created: SBO 2011-05-09
// =============================================================================
class PluginSetting : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginSetting( QWidget* parent, xml::xistream& xis );
    virtual ~PluginSetting();
    //@}

    //! @name Operations
    //@{
    void Accept( PluginSettingVisitor_ABC& visitor );
    //@}

private:
    //! @name Member data
    //@{
    const std::string attribute_;
    const std::string type_;
    QLineEdit* stringValue_;
    QSpinBox* integerValue_;
    QCheckBox* booleanValue_;
    QTimeEdit* timeValue_;
    //@}
};

}

#endif // __PluginSetting_h_
