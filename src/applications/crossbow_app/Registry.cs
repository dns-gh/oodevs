using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;

namespace Sword
{
    namespace Crossbow
    {
        class Registry
        {
            // private string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            static public void Register(Type registerType, string component)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                Microsoft.Win32.Registry.ClassesRoot.CreateSubKey(regKey + "\\Implemented Categories\\" + component);
            }

            static public void Unregister(Type registerType, string component)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                Microsoft.Win32.Registry.ClassesRoot.DeleteSubKey(regKey + "\\Implemented Categories\\" + component);
            }

            // ArcMap
            /*
            public const string COMPONENT_MX_COMMAND = "{1B422713-6E35-393C-8C17-92F3B9B5AA3C}";
            public const string COMPONENT_MX_COMMANDBARS = "{1B422713-6E35-393C-8C17-92F3B9B5AA3C}";
            public const string COMPONENT_MX_DOCKABLEWINDOW = "{85A922A5-0937-3D43-AABD-06213787CDBE}";
            public const string COMPONENT_MX_EXTENSION = "{976D1371-61F5-3635-B2AA-4D866928A2BB}";
            */
            
            public const string COMPONENT_MX_COMMAND = "{B56A7C42-83D4-11D2-A2E9-080009B6F22B}";
            public const string COMPONENT_MX_TOOLBAR     = "{B56A7C4A-83D4-11D2-A2E9-080009B6F22B}";
            public const string COMPONENT_MX_COMMANDBARS = "{B56A7C4A-83D4-11D2-A2E9-080009B6F22B}";
            public const string COMPONENT_MX_DOCKABLEWINDOW = "{117623B5-F9D1-11D3-A67F-0008C7DF97B9}";
            public const string COMPONENT_MX_TOCVIEW = "{089874FC-CC18-11D2-9F39-00C04F6BC78E}";
            public const string COMPONENT_MX_EXTENSION = "{B56A7C45-83D4-11D2-A2E9-080009B6F22B}";
            
            // ArcGlobe
            internal const string COMPONENT_GMX_COMMAND = "{720E21D4-2199-11D6-B2B3-00508BCDDE28}";
            internal const string COMPONENT_GMX_TOOLBAR = "{720E21D3-2199-11D6-B2B3-00508BCDDE28}";
            internal const string COMPONENT_GMX_DOCKABLEWINDOW = "{720E21D8-2199-11D6-B2B3-00508BCDDE28}";
            internal const string COMPONENT_GMX_JITEXTENSION = "{99A23410-D290-41C8-83D8-123D37A1B67B}";
            internal const string COMPONENT_GMX_ANIMATION_TYPE = "{350DE633-42A4-458D-B07B-35D3B10E12B6}";
        }
    }
}