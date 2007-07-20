using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public sealed class OrderParameter : IOrderParameter
    {
        private OrderManager m_orderManager;
        private string m_typeID;
        private string m_name;
        private object m_param;
                
        public OrderParameter(string name, string typeID)
        {
            m_name = name;
            m_typeID = typeID;
        }

        public void OnCreate(OrderManager hook)
        {
            m_orderManager = hook;
        }

        public void Serialize(ITable table, int oid)
        {
            IRow row = table.CreateRow();
            
            Tools.SetValue(row, "order_id", oid);
            Tools.SetValue(row, "name", m_name);
            Tools.SetValue(row, "context", false);
            Tools.SetValue(row, "ParamValue", "");
            row.Store();
        }

        public void OnContextMenu(ICommandBar cmdBar, int x, int y, IFeature selected)
        {
            if ( selected != null )
                OnContextMenuFeature(cmdBar, selected);
            else
                OnContextMenuInternal(cmdBar, x, y, selected);            
        }

        public void OnContextMenuFeature(ICommandBar cmdBar, IFeature selected)
        {
            // if ( typeId == "shape" && selected.Class.
            // cmdBar.
            // if ( typeId == "automat" && selected.Class.
        }

        public void OnContextMenuInternal(ICommandBar cmdBar, int x, int y, IFeature selected)
        {
            /*
            if ( typeID == "path" )
                cmdBar.Add( "SetPath" );
            if ( typeID == "point" )
                cmdBar.Add( "SetPath" );
            if ( typeID == "bool" )
                cmdBar.Add( "Toogle" );
             */
        }

        public string Name
        {
            get
            {
                return m_name;
            }
        }
    }
}
