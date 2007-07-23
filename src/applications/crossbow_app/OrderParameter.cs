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
        private string m_type;
        private string m_name;
                
        public OrderParameter(string name, string type)
        {
            m_name = name;
            m_type = type;
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
