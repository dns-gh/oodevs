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
#include <QtCore/qobject.h>

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QTimeEdit;
class QWidget;
class QComboBox;
class QPushButton;
class Q3TimeEdit;


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

    class FileButtonEvent 
        : public QObject, public boost::noncopyable
    {
        Q_OBJECT;
    
    public:
        explicit FileButtonEvent( PluginSetting& plugins, QWidget* parent, const std::string& description );
        void setText( const std::string& value );
        
    private slots:  
        //! @name Slots
        //@{
        void OnFileClicked();

    private:
        //! @name 
        //@{
        QPushButton* fileValue_;
        PluginSetting& plugins_;
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
    void Accept( PluginSettingVisitor_ABC& visitor );
    //@}

private:
    friend class FileButtonEvent;
    void OnFileClicked();

private:
    //! @name Member data
    //@{
    const std::string attribute_;
    const std::string type_;
    QLineEdit* stringValue_;
    QSpinBox* integerValue_;
    QCheckBox* booleanValue_;
    Q3TimeEdit* timeValue_;
    QComboBox* enumerationValue_;
    std::string fileName_;
    std::auto_ptr< FileButtonEvent > fileValue_;
    const tools::GeneralConfig& config_;
    //@}
};

}

#endif // __PluginSetting_h_
