using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ADF.CATIDs;

namespace Sword
{
    namespace Crossbow
    {
        [ComVisible(true)]
        [Guid("1810c551-42e8-4530-bfe0-0fb91d0f634f")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.SwordExtension")]
        public class SwordExtension : IExtension, IExtensionConfig, IPersistVariant
        {
            #region COM Registration Function(s)
            [ComRegisterFunction()]
            [ComVisible(false)]
            static void RegisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryRegistration(registerType);
                //Registry.Register(registerType, //Registry.COMPONENT_MX_EXTENSION);
            }

            [ComUnregisterFunction()]
            [ComVisible(false)]
            static void UnregisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryUnregistration(registerType);
                //Registry.Unregister(registerType, //Registry.COMPONENT_MX_EXTENSION);
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
                GMxExtensions.Register(regKey);

            }
            /// <summary>
            /// Required method for ArcGIS Component Category unregistration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryUnregistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxExtension.Unregister(regKey);
                GMxExtensions.Unregister(regKey);
            }

            #endregion
            #endregion

            private WorkspaceConfiguration m_config = new WorkspaceConfiguration();
            private StaticModel m_model;
            private OrderHandler m_orderHandler;
            private EmergencyHandler m_emergencyHandler;
            private SymbolFactory m_symbolFactory = new SymbolFactory();
            private esriExtensionState m_Enabled;

            private IDocumentEvents_NewDocumentEventHandler m_newDocumentEvent;
            private IDocumentEvents_OpenDocumentEventHandler m_openDocumentEvent;
            private IDocumentEvents_CloseDocumentEventHandler m_closeDocumentEvent;
            public event EventHandler ModelLoaded;

            #region IExtension Members
            /// <summary>
            /// Name of extension. Do not exceed 31 characters
            /// </summary>
            public string Name
            {
                get
                {
                    return "SwordExtension";
                }
            }

            public void Shutdown()
            {
                m_emergencyHandler = null;
                m_orderHandler = null;
                m_symbolFactory = null;
                m_config = null;
                m_model = null;
                m_newDocumentEvent = null;
                m_openDocumentEvent = null;
                m_closeDocumentEvent = null;
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
                m_newDocumentEvent = new IDocumentEvents_NewDocumentEventHandler(OnNewDocumentHandler);
                ((IDocumentEvents_Event)(Tools.GetIDocument())).NewDocument += m_newDocumentEvent;
                m_openDocumentEvent = new IDocumentEvents_OpenDocumentEventHandler(OnOpenDocumentHandler);
                ((IDocumentEvents_Event)(Tools.GetIDocument())).OpenDocument += m_openDocumentEvent;
                m_closeDocumentEvent = new IDocumentEvents_CloseDocumentEventHandler(OnCloseDocumentHandler);
                ((IDocumentEvents_Event)(Tools.GetIDocument())).CloseDocument += m_closeDocumentEvent;
            }

            private void OnNewDocumentHandler()
            {
                Tools.GetDocument().EnableDynamicDisplay(true);
            }

            private void OnOpenDocumentHandler()
            {
                Tools.GetDocument().EnableDynamicDisplay(true);
            }

            private void OnCloseDocumentHandler()
            {
                Tools.GetDocument().EnableDynamicDisplay(false);
            }

            public void NotifyModelLoaded()
            {
                m_model = new StaticModel(m_config);
                if (m_orderHandler == null)
                    m_orderHandler = new OrderHandler(m_model);
                if (m_emergencyHandler == null)
                    m_emergencyHandler = new EmergencyHandler(m_model);
                ModelLoaded(this, EventArgs.Empty);
                ParameterTypeFactory.Initialize(Config.SharedFile);
            }

            public OrderHandler OrderHandler
            {
                get
                {
                    return m_orderHandler;
                }
            }

            public EmergencyHandler EmergencyHandler
            {
                get
                {
                    return m_emergencyHandler;
                }
            }

            public WorkspaceConfiguration Config
            {
                get
                {
                    return m_config;
                }
            }

            public SymbolFactory SymbolFactory
            {
                get
                {
                    return m_symbolFactory;
                }
            }
            #endregion

            #region IPersistVariant Members

            public UID ID
            {
                get
                {
                    UID typeID = new UIDClass();
                    typeID.Value = GetType().GUID.ToString("B");
                    return typeID;
                }
            }

            public void Load(IVariantStream Stream)
            {
                //TODO: Load persisted data from document stream

                Marshal.ReleaseComObject(Stream);
            }

            public void Save(IVariantStream Stream)
            {
                //TODO: Save extension related data to document stream

                Marshal.ReleaseComObject(Stream);
            }

            #endregion

            #region IExtensionConfig Members

            public string Description
            {
                get 
                {
                    return "Provide simulation communication utilities.";
                }
            }

            public string ProductName
            {
                get 
                { 
                    return "Sword Urban Emergency - ESRI Plugin"; 
                }
            }

            public esriExtensionState State
            {
                get
                {
                    return m_Enabled;
                }
                set
                {
                    // Check if ok to enable or disable extension
                    esriExtensionState requestState = value;
                    if (requestState == esriExtensionState.esriESEnabled)
                    {
                        // Cannot enable if it's already in unavailable state
                        if (m_Enabled == esriExtensionState.esriESUnavailable)
                            throw new COMException("Not running the appropriate product license.");
                        // Determine if state can be changed
                        m_Enabled = requestState;
                    }
                    else
                    {
                        m_Enabled = requestState;
                    }
                }
            }

            #endregion
        }
    }
}