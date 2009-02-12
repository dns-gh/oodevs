using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;

namespace Sword
{
    namespace Crossbow
    {
        [ComVisible(true)]
        [Guid("02a14a61-dd9f-493e-80be-7927ae8a5bac")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.Orbat")]
        public partial class Orbat : UserControl, IDockableWindowDef
        {
            private SymbolFactory m_pSymbolFactory = new SymbolFactory();
            private IDisplay m_SimpleDisplay = new SimpleDisplayClass();

            #region COM Registration Function(s)
            [ComRegisterFunction()]
            [ComVisible(false)]
            static void RegisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryRegistration(registerType);
                //Registry.Register(registerType, //Registry.COMPONENT_MX_DOCKABLEWINDOW);
            }

            [ComUnregisterFunction()]
            [ComVisible(false)]
            static void UnregisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryUnregistration(registerType);
                //Registry.Unregister(registerType, //Registry.COMPONENT_MX_DOCKABLEWINDOW);
            }

            #region ArcGIS Component Category Registrar generated code
            /// <summary>
            /// Required method for ArcGIS Component Category registration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryRegistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxDockableWindows.Register(regKey);
                GMxDockableWindows.Register(regKey);
            }
            /// <summary>
            /// Required method for ArcGIS Component Category unregistration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryUnregistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxDockableWindows.Unregister(regKey);
                GMxDockableWindows.Unregister(regKey);
            }

            #endregion
            #endregion

            public Orbat()
            {
                InitializeComponent();
                InitializeEvents();
            }

            #region IDockableWindowDef Members

            public string Caption
            {
                get
                {
                    return "Order of Battle";
                }
            }

            public int ChildHWND
            {
                get { return this.Handle.ToInt32(); }
            }

            public void OnCreate(object hook)
            {
                Tools.GetSwordExtension().ModelLoaded += new EventHandler(OnModelLoadedHandler);
            }

            public void OnDestroy()
            {
                m_SimpleDisplay = null;
                m_pSymbolFactory = null;
            }

            public object UserData
            {
                get { return this; }
            }

            #endregion

            private void OnModelLoadedHandler(object sender, EventArgs e)
            {
                LoadLayer(Tools.GetSwordExtension().Config.LayersConfiguration.Units);
            }
        }
    }
}
