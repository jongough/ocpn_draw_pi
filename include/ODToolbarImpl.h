#ifndef __ODToolbarImpl__
#define __ODToolbarImpl__

/**
@file
Subclass of MyFrameODToolbar, which is generated by wxFormBuilder.
*/

#include "ODToolbarDef.h"

enum {
    ID_BOUNDARY = 1000,
    ID_ODPOINT,
    ID_TEXTPOINT,
    ID_TOOLBARPANEL,
    
    ID_TOOL_DEF_LAST
};

/** Implementing MyFrameODToolbar */
class ODToolbarImpl : public ODToolbarDialog
{
public:
	/** Constructor */
	ODToolbarImpl( wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style );
    ~ODToolbarImpl();
    
    wxToolBarToolBase *m_toolBoundary;
    
};

#endif // __ODToolbarImpl__
