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

namespace crossbow
{
    public interface IOrbat
    {
        void Load();
        void Unload();
    }

    [Guid("02a14a61-dd9f-493e-80be-7927ae8a5bac")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.Orbat")]
    public partial class Orbat : UserControl, IDockableWindowDef, IOrbat
    {
        private IApplication m_application;        
        private SymbolFactory m_pSymbolFactory = new SymbolFactory();
        private IDisplay m_SimpleDisplay = new SimpleDisplayClass();
        private bool m_IsLoaded = false;
        private FieldsProperty m_Fields = new FieldsProperty();

        #region COM Registration Function(s)
        [ComRegisterFunction()]
        [ComVisible(false)]
        static void RegisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryRegistration(registerType);
            //
            // TODO: Add any COM registration code here
            //
        }

        [ComUnregisterFunction()]
        [ComVisible(false)]
        static void UnregisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryUnregistration(registerType);

            //
            // TODO: Add any COM unregistration code here
            //
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

        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxDockableWindows.Unregister(regKey);

        }

        #endregion
        #endregion
                
        public Orbat()
        {            
            InitializeComponent();
        }
        
        #region IDockableWindowDef Members

        public string Caption
        {
            get
            {
                //TODO: Replace with locale-based initial title bar caption
                return "My C# Dockable Window";
            }
        }

        public int ChildHWND
        {
            get { return this.Handle.ToInt32(); }
        }

        public void OnCreate(object hook)
        {
            m_application = hook as IApplication;
        }

        public void OnDestroy()
        {
            //TODO: Release resources and call dispose of any ActiveX control initialized
        }

        public object UserData
        {
            get { return this; }
        }

        #endregion

        #region IOrbat Members

        public new void Load()
        {
            if (!m_IsLoaded)
                SetupOrbatCommand();
            m_IsLoaded = true;
        }

        public void Unload()
        {
            // $$$$ SBO 2007-07-11: cleaning...
        }

        #endregion
    }
}
