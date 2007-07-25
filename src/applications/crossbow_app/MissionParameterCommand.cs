using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ArcMapUI;

namespace crossbow
{
    #region Mission - Base    
    public class ParameterCommand : BaseCommand
    {
        #region COM Registration Function(s)
        [ComRegisterFunction()]
        [ComVisible(false)]
        static void RegisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryRegistration(registerType);
        }

        [ComUnregisterFunction()]
        [ComVisible(false)]
        static void UnregisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryUnregistration(registerType);
        }

        #region ArcGIS Component Category Registrar generated code
        /// <summary>
        /// Required method for ArcGIS Component Category registration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryRegistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxCommands.Register(regKey);
        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxCommands.Unregister(regKey);
        }

        #endregion
        #endregion

        private IApplication m_application;
        public ParameterCommand(string category, string name)
        {            
            base.m_category = category; //localizable text
            base.m_caption = name;  //localizable text
            base.m_message = name;  //localizable text 
            base.m_toolTip = name;  //localizable text 
            base.m_name = name;   //unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")
            
            try
            {
                string bitmapResourceName = GetType().Name + ".bmp";
                base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
            }
        }        
        #region Overriden Class Methods
        /// <summary>
        /// Occurs when this command is created
        /// </summary>
        /// <param name="hook">Instance of the application</param>
        public override void OnCreate(object hook)
        {
            if (hook == null)
                return;

            m_application = hook as IApplication;            
            //Disable if it is not ArcMap
            if (hook is IMxApplication)
                base.m_enabled = true;
            else
                base.m_enabled = false;
        }

        /// <summary>
        /// Occurs when this command is clicked
        /// </summary>
        public override void OnClick()
        {
//            CSwordExtension extension = Tools.GetCSwordExtension(app);
 //           extension.OrderManager.RegisterLima(m_name);
        }

        #endregion

        #region LD
            [Guid("F977CBFE-AF76-46c7-BFB1-1882E143293C")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLD")]
            public sealed class MissionParamater_LimaLD : ParameterCommand
            {
                MissionParamater_LimaLD() : base("MissionParamater", "LD") { }
            }
        #endregion
        #region LC
            [Guid("AD7505EE-0C41-4d2b-9039-BEDB15AA52F6")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLC")]
            public sealed class MissionParamater_LimaLC : ParameterCommand
            {
                MissionParamater_LimaLC() : base("MissionParamater", "LC") { }
            }
        #endregion
        #region LCA
            [Guid("19714A99-07ED-4dae-BA1B-568D9AFCE7BF")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLCA")]
            public sealed class MissionParamater_LimaLCA : ParameterCommand
            {
                MissionParamater_LimaLCA() : base("MissionParamater", "LCA") { }
            }
        #endregion
        #region LI
            [Guid("A0DA5CAE-108C-4a82-AE5E-07E116D3CCCF")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLI")]
            public sealed class MissionParamater_LimaLI : ParameterCommand
            {
                MissionParamater_LimaLI() : base("MissionParamater", "LI") { }
            }
        #endregion
        #region LO
            [Guid("D19CCE8B-7ED5-40cb-AC38-44D1019D30E9")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLO")]
            public sealed class MissionParamater_LimaLO : ParameterCommand
            {
                MissionParamater_LimaLO() : base("MissionParamater", "LO") { }
            }
        #endregion
        #region LCAR
            [Guid("EF8DE37F-8272-47f1-A3DC-BF633A041A73")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLCAR")]
            public sealed class MissionParamater_LimaLCAR : ParameterCommand
            {
                MissionParamater_LimaLCAR() : base("MissionParamater", "LCAR") { }
            }
        #endregion
        #region LR
            [Guid("598E3CF5-97EF-4402-B0E8-F5049A82156C")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLR")]
            public sealed class MissionParamater_LimaLR : ParameterCommand
            {
                MissionParamater_LimaLR() : base("MissionParamater", "LR") { }
            }
        #endregion
        #region LDM
            [Guid("973B3F2A-D6D5-44fc-8509-745ED7BBE0D4")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLDM")]
            public sealed class MissionParamater_LimaLDM : ParameterCommand
            {
                MissionParamater_LimaLDM() : base("MissionParamater", "LDM") { }
            }
        #endregion
        #region LFM
            [Guid("527D20FA-94E9-4116-919A-11364ADC37EA")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLFM")]
            public sealed class MissionParamater_LimaLFM : ParameterCommand
            {
                MissionParamater_LimaLFM() : base("MissionParamater", "LFM") { }
            }
        #endregion
        #region LIA
            [Guid("F565C162-7D10-4ce6-8326-2E78A7AA8EEB")]
            [ClassInterface(ClassInterfaceType.None)]
            [ProgId("crossbow.MissionParamater_LimaLIA")]
            public sealed class MissionParamater_LimaLIA : ParameterCommand
            {
                MissionParamater_LimaLIA() : base("MissionParamater", "LIA") { }
            }
        #endregion
    }
    #endregion
}
