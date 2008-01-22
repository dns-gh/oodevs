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
        /// Summary description for CSwordMenu.
        /// </summary>
        [ComVisible(true)]
        [Guid("2eb6ae6d-2b52-40e3-9177-0b2d43a793fe")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.SwordMenu")]
        public sealed class SwordMenu : BaseMenu
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

            public SwordMenu()
            {
                AddItem("Sword.Crossbow.OrbatCommand");
                AddItem("Sword.Crossbow.InfoPanelCommand");
            }

            public override string Caption
            {
                get
                {
                    return "Sword";
                }
            }
            public override string Name
            {
                get
                {
                    return "Sword_Crossbow_SwordMenu";
                }
            }
        }
    }
}