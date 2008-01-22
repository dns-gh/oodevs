using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.ADF.BaseClasses;

namespace Sword
{
    namespace Crossbow
    {
        /// <summary>
        /// Summary description for CSwordBar.
        /// </summary>
        [ComVisible(true)]
        [Guid("666b9717-13e0-47d0-a388-7b3b7b581342")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.SwordBar")]
        public sealed class SwordBar : BaseToolbar
        {
            #region COM Registration Function(s)
            [ComRegisterFunction()]
            [ComVisible(false)]
            static void RegisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryRegistration(registerType);
                //Registry.Register(registerType, //Registry.COMPONENT_MX_TOOLBAR);
            }

            [ComUnregisterFunction()]
            [ComVisible(false)]
            static void UnregisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryUnregistration(registerType);
                //Registry.Unregister(registerType, //Registry.COMPONENT_MX_TOOLBAR);
            }

            #region ArcGIS Component Category Registrar generated code
            /// <summary>
            /// Required method for ArcGIS Component Category registration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryRegistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxCommandBars.Register(regKey);
                GMxCommandBars.Register(regKey);
            }
            /// <summary>
            /// Required method for ArcGIS Component Category unregistration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryUnregistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxCommandBars.Unregister(regKey);
                GMxCommandBars.Unregister(regKey);
            }

            #endregion
            #endregion

            public SwordBar()
            {
                AddItem("Sword.Crossbow.SwordMenu");
                AddItem("Sword.Crossbow.CreateWorkspace");
                AddItem("Sword.Crossbow.AddDynamicMoleLayer");
                AddItem("Sword.Crossbow.OrbatCommand");
                AddItem("Sword.Crossbow.OrderTool");
                AddItem("Sword.Crossbow.CreateEventTool");
                AddItem("Sword.Crossbow.FilterViewTool");
            }

            public override string Caption
            {
                get
                {
                    return "SwordBar";
                }
            }
            public override string Name
            {
                get
                {
                    return "SwordBar";
                }
            }
        }
    }
}