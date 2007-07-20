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
    public class MissionOrderCommand : BaseCommand
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
        public MissionOrderCommand( string category, string name )
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
            MissionOrderForm form = new MissionOrderForm(m_name, m_application);            
            form.Show();
        }

        #endregion
    }
    #endregion

    #region Armor - To Attack
        [Guid("B8A3F756-A59A-4ca4-A21D-D712577EADE7")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("crossbow.MissionOrder_ArmorAttack")] 
        public sealed class MissionOrder_ArmorAttack : MissionOrderCommand
        {
            public MissionOrder_ArmorAttack() : base( "Mission", "Armor - To attack" ) {}
        }
    #endregion
    #region Armor - To support (dynamic)
        [Guid("62B9A779-17E6-4497-A245-ED2298662A01")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("crossbow.MissionOrder_ArmorSupport")]
        public sealed class MissionOrder_ArmorSupport : MissionOrderCommand
        {
            public MissionOrder_ArmorSupport() : base( "Mission", "Armor - To support (dynamic)" ) {}
        }
    #endregion
    #region Armor - To hold
        [Guid("D16317DE-36F6-44ca-A251-DC8B8CEC69D2")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("crossbow.MissionOrder_ArmorHold")]
        public sealed class MissionOrder_ArmorHold : MissionOrderCommand
        {
            public MissionOrder_ArmorHold() : base( "Mission", "Armor - To hold" ) {}
        }
    #endregion
    #region Infantry - To reconnoiter
        [Guid("ABD76DE7-470A-4775-A4CC-3A144FD58457")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("crossbow.MissionOrder_InfanteryReconnoiter")]    
        public sealed class MissionOrder_InfanteryReconnoiter : MissionOrderCommand
        {
            public MissionOrder_InfanteryReconnoiter() : base( "Mission", "Infantery - To reconnoiter" ) {}
        }
    #endregion
    #region Infantry - To deny
        [Guid("6542E42B-07AF-4660-8001-7874B414A9FD")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("crossbow.MissionOrder_InfanteryDeny")]
        public sealed class MissionOrder_InfanteryDeny : MissionOrderCommand
        {
            public MissionOrder_InfanteryDeny() : base( "Mission", "Infantery - To deny" ) {}
        }
    #endregion       

}
