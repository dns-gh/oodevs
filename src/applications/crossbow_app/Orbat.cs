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
        void Update();
        void Unload();
    }

    [Guid("02a14a61-dd9f-493e-80be-7927ae8a5bac")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("crossbow.Orbat")]
    public partial class Orbat : UserControl, IDockableWindowDef, IOrbat
    {
        private IApplication m_application;        
        private SymbolFactory m_pSymbolFactory;
        private IDisplay m_SimpleDisplay;

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

        #region "Fields definition"
        private enum EnumFields { eOID, eParentOID, eName, eSymbol, eFormationOID };
        private int[] m_formationFields;
        private int[] m_tacticalElementFields;

        public void SetupFormationFields(ITable pTable)
        {
            m_formationFields[(int)EnumFields.eSymbol] = pTable.FindField("Symbol_ID");
            m_formationFields[(int)EnumFields.eOID] = pTable.FindField("Public_OID");
            m_formationFields[(int)EnumFields.eParentOID] = pTable.FindField("Parent_OID");
            m_formationFields[(int)EnumFields.eName] = pTable.FindField("Name");
        }

        public void SetupTacticalElementFields(IFeatureClass pFeatureClass)
        {
            m_tacticalElementFields[(int)EnumFields.eSymbol] = pFeatureClass.FindField("Symbol_ID");
            m_tacticalElementFields[(int)EnumFields.eOID] = pFeatureClass.FindField("Public_OID");
            m_tacticalElementFields[(int)EnumFields.eParentOID] = pFeatureClass.FindField("Parent_OID");
            m_tacticalElementFields[(int)EnumFields.eName] = pFeatureClass.FindField("Name");
            m_tacticalElementFields[(int)EnumFields.eFormationOID] = pFeatureClass.FindField("Formation_OID");
        }
        #endregion

        public Orbat()
        {
            m_SimpleDisplay = new SimpleDisplayClass();
            m_pSymbolFactory = new SymbolFactory();
            m_formationFields = new int[4];
            m_tacticalElementFields = new int[5];
            InitializeComponent();
        }

        #region IOrbat Members
        void IOrbat.Load()
        {
            SetupOrbatCommand();
        }

        void IOrbat.Unload()
        {
            // SetupOrbatCommand();
            throw new Exception("The method or operation is not implemented.");
        }

        void IOrbat.Update()
        {
            throw new Exception("The method or operation is not implemented.");
        }
        #endregion

        

        #region IDockableWindowDef Members

        string IDockableWindowDef.Caption
        {
            get
            {
                //TODO: Replace with locale-based initial title bar caption
                return "My C# Dockable Window";
            }
        }

        int IDockableWindowDef.ChildHWND
        {
            get { return this.Handle.ToInt32(); }
        }

        string IDockableWindowDef.Name
        {
            get
            {
                //TODO: Replace with any non-localizable string
                return this.Name;
            }
        }

        void IDockableWindowDef.OnCreate(object hook)
        {
            m_application = hook as IApplication;            
        }

        void IDockableWindowDef.OnDestroy()
        {
            //TODO: Release resources and call dispose of any ActiveX control initialized
        }

        object IDockableWindowDef.UserData
        {
            get { return this; }
        }

        #endregion           
    }
}
