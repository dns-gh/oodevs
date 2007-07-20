using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;

namespace crossbow
{
    public class OrderFactory
    {
        private IMxDocument m_mxDocument;
        private System.Collections.Hashtable m_model = new System.Collections.Hashtable();
        private FieldsProperty m_Fields = new FieldsProperty();

        private class ModelParameters
        {
            private System.Collections.Hashtable parameters = new System.Collections.Hashtable();

            public ModelParameters()
            {
            }

            public void Register(string name, string typeID)
            {
                parameters.Add(name, typeID);
            }

            public void Run(Order order)
            {
                System.Collections.IDictionaryEnumerator it = parameters.GetEnumerator();
                while ( it.MoveNext() )                
                    order.RegisterParameter(it.Key.ToString(), it.Value.ToString());                
            }
        }
        private class ModelMission
        {
        }

        public OrderFactory(IMxDocument mxDocument)
        {
            m_mxDocument = mxDocument;
        }

        public void OnCreate()
        {
            m_model.Add("Armor - To attack", new ModelParameters());

            {
                ModelParameters param = new ModelParameters();
                param.Register("Deployment area", "shape");
                m_model.Add("Armor - To hold", param);
            }
            {
                ModelParameters param = new ModelParameters();
                param.Register("Unit", "automat");
                m_model.Add("Armor - To support (dynamic)", param);
            }
            {
                ModelParameters param = new ModelParameters();
                param.Register("Position to deny", "point");
                param.Register("Obstacle erection", "bool");
                m_model.Add("Infantery - To deny", param);
            }
            m_model.Add("Infantery - To reconnoiter", new ModelParameters());
        }

        public void OpenMissionContextMenu(int x, int y)
        {
            IDocument doc = (IDocument)m_mxDocument;
            ICommandBar cmdBar = doc.CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);

            BuildMissionContextMenu(cmdBar);            
            cmdBar.Popup(x, y);
        }

        public void OpenMissionParameterContextMenu(int x, int y)
        {
            IDocument doc = (IDocument)m_mxDocument;
            ICommandBar cmdBar = doc.CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
            
            cmdBar.Popup(x, y);
        }

        void BuildMissionContextMenu(ICommandBar cmdBar)
        {
            object Missing = Type.Missing;
            ICommandBar cmdSubMenu = cmdBar.CreateMenu("Missions", ref Missing);
            
            UID menuUID = new UID();
            menuUID.Value = "crossbow.MissionOrder_ArmorAttack";
            cmdSubMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_ArmorSupport";            
            cmdSubMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_ArmorHold";
            cmdSubMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_InfanteryReconnoiter";
            cmdSubMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_InfanteryDeny";
            cmdSubMenu.Add(menuUID, ref Missing);
        }

        public void BuildParameters(Order order)
        {
            ModelParameters parameters = (ModelParameters)m_model[order.Name];
            
            if (parameters != null)
                parameters.Run(order);            
        }
    }
}
