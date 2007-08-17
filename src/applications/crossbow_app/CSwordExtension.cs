using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ADF.CATIDs;

namespace Crossbow
{
    [ComVisible(true)]
    [Guid("1810c551-42e8-4530-bfe0-0fb91d0f634f")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.CSwordExtension")]
    public class CSwordExtension : IExtension
    {
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
            MxExtension.Register(regKey);

        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxExtension.Unregister(regKey);

        }

        #endregion
        #endregion

        private WorkspaceConfiguration m_config = new WorkspaceConfiguration();
        private OrderHandler m_orderHandler;

        private IDocumentEvents_OnContextMenuEventHandler m_contextMenuEvent;
        private WorkspaceConfiguration_ConfigurationLoadedEventHandler m_configLoadedEvent;

        #region IExtension Members
        /// <summary>
        /// Name of extension. Do not exceed 31 characters
        /// </summary>
        public string Name
        {
            get
            {        
                return "CSwordExtension";
            }
        }

        public void Shutdown()
        {
            m_orderHandler = null;
        }

        public void Startup(ref object initializationData)
        {
            IApplication app = initializationData as IApplication;
            if (app == null)
                return;
            Tools.Initialize(app);
            SetupEvents();
        }

        private void SetupEvents()
        {
            {
                m_configLoadedEvent = new WorkspaceConfiguration_ConfigurationLoadedEventHandler(OnConfigurationLoadedHandler);
                m_config.ConfigurationLoaded += m_configLoadedEvent;
            }
            {
                m_contextMenuEvent = new IDocumentEvents_OnContextMenuEventHandler(OnContextMenuHandler);
                ((IDocumentEvents_Event)(Tools.GetMxDocument())).OnContextMenu += m_contextMenuEvent;
            }
        }

        private void OnConfigurationLoadedHandler()
        {
            if (m_orderHandler == null)
                m_orderHandler = new OrderHandler(m_config);
        }

        private void OnContextMenuHandler(int x, int y, out bool handled)
        {
            handled = false;
            if (m_orderHandler != null)
                handled = m_orderHandler.OnContextMenu(x, y);
        }
        
        public OrderHandler OrderHandler
        {
            get
            {
                return m_orderHandler;
            }
        }

        public WorkspaceConfiguration Config
        {
            get
            {
                return m_config;
            }
        }
        #endregion
    }
}
