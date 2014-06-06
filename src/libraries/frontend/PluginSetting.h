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
#include "clients_kernel/XmlDescription.h"
#pragma warning( push, 0 )
#include <QtCore/QObject>
#include <QtGui/QPushButton>
#pragma warning( pop )

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QTimeEdit;
class QWidget;
class QComboBox;
class QPushButton;
class QLabel;

namespace tools
{
    class GeneralConfig;
}

namespace xml
{
    class xistream;
}

namespace frontend
{
    class PluginSettingVisitor_ABC;
    class PluginSetting;
    class FileList;


class FileButtonEvent : public QPushButton
                      , public boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileButtonEvent( PluginSetting& plugins );
    //@}

    //! @name Operation
    //@{
    bool HasBeenUpdated() const;
    void SetText( const std::string& value );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnFileClicked();
    //@}

private:
    //! @name 
    //@{
    PluginSetting& plugins_;
    bool           updated_;
    //@}
};

// =============================================================================
/** @class  PluginSetting
    @brief  Plugin setting
*/
// Created: SBO 2011-05-09
// =============================================================================
class PluginSetting : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PluginSetting( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis );
    virtual ~PluginSetting();
    //@}

    //! @name Operations
    //@{
    void OnLanguageChanged();
    void Accept( PluginSettingVisitor_ABC& visitor );
    //@}

private:
    friend class FileButtonEvent;
    void OnFileClicked();

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&      config_;
    const std::string                attribute_;
    const std::string                type_;
    kernel::XmlDescription           description_;
    QLabel*                          label_;
    QLineEdit*                       stringValue_;
    QSpinBox*                        integerValue_;
    QCheckBox*                       booleanValue_;
    QTimeEdit*                       timeValue_;
    QComboBox*                       enumerationValue_;
    FileList*                        fileList_;
    std::string                      fileName_;
    std::unique_ptr< FileButtonEvent > fileValue_;
    //@}
};

}

#endif // __PluginSetting_h_
