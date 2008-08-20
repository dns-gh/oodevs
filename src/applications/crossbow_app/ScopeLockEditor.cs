using ESRI.ArcGIS.Geodatabase;

namespace Sword
{
    namespace Crossbow
    {
        /*
         * Enable IFeatureWorkspace edition during instance life
         */
        class ScopeLockEditor
        {
            IWorkspaceEdit m_edit;

            public ScopeLockEditor(IDataset dataset)
            {
                m_edit = (IWorkspaceEdit)dataset.Workspace;
            }
            public void Lock()
            {
                if (!m_edit.IsBeingEdited())
                    m_edit.StartEditing(true);
                m_edit.StartEditOperation();
            }
            public void Abord()
            {
                m_edit.AbortEditOperation();
            }
            public void Unlock()
            {
                m_edit.StopEditOperation();
                if (m_edit.IsBeingEdited())
                    m_edit.StopEditing(true);
            }
        }
    }
}